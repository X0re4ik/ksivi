# Архитектура robolaunch Cloudy

Одностраничная карта системы: какие блоки есть, как данные по ним гуляют,
что где запускается. Здесь только то, что **нельзя выведать из CLI** (`ros2 node list`,
`rqt_graph`) — живую перепись узлов и деталей сообщений смотрите там.

> Все пакеты собираются из одного workspace командой `colcon build`.
> Команды запуска — в конце документа.

## Слои системы

```
┌─────────────────────────────────────────────────────────┐
│  ПРИЛОЖЕНИЯ (что робот делает)                          │
│    Nav2 (доехать до точки) · слежение · патрулирование   │
├─────────────────────────────────────────────────────────┤
│  МОЗГ (восприятие мира)                                 │
│    SLAM (карта) · EKF (фильтр одометрии) · Nav2 costmap  │
├─────────────────────────────────────────────────────────┤
│  ТЕЛО И ГЛАЗА (сенсоры и привод)                        │
│    лидар /scan · камера /camera/* · IMU · diff_drive     │
├─────────────────────────────────────────────────────────┤
│  БАЗА (описание и время)                                │
│    /robot_description (URDF) · /tf · /clock             │
└─────────────────────────────────────────────────────────┘
```

## Поток данных (сердце системы)

```
  /cmd_vel (Twist: v вперёд + ω поворот)
      │
      ▼
  «ноги» (в симуляции: плагин diff_drive,        ◀── симуляция
    на реальном роботе: ros2_control + ESP32)  ◀── реальный робот
      │
      ▼  механика едет
      │
      ├──▶ /scan (лидар, 10 Гц, 360 лучей)    ← восприятие мира
      ├──▶ /camera/* (RGB-D)
      ├──▶ /tf (300 Гц, где что стоит)
      │
      ▼  [МОЗГ] (SLAM + EKF подпишутся на эти данные здесь)
      │
      ▼ обратно в /cmd_vel → цикл замкнулся
```

## Ключевые топики (кого не видеть нельзя)

| Топик | Роль | Примечание |
|---|---|---|
| `/cmd_vel` | **Вход** управления | Единственный рычаг движения: linear.x + angular.z |
| `/scan` | Лидар | 360 расстояний, 10 Гц; «монополист» восприятия для SLAM |
| `/tf`, `/tf_static` | Позы | Дерево координат: `odom → base_footprint → колёса/сенсоры` |
| `/camera/*` | RGB-D | 1 сенсор → 11 каналов (форматы сжатия), не путать с 11 сенсорами |
| `/joint_states` | Углы звеньев | **В симуляции издателя нет** (см. «Неочевидное» ниже) |
| `/clock` | Время симуляции | Только в симе; на реальном роботе — время host-машины |
| `/map` (позже) | Карта из SLAM | Появится при запуске SLAM/Nav2 |
| `/plan`, `/local_costmap/*` (позже) | Планы Nav2 | Подписчики Nav2 |

## Соответствие «симуляция ↔ реальный робот»

**Архитектура не меняется, меняется только «нижний этаж»:**

| Роль | В симуляции | На реальном роботе |
|---|---|---|
| Физический мир | `gazebo.launch.py` (Gazebo) | — |
| «Ноги» | плагин `libgazebo_ros_diff_drive` в `description/urdf/gazebo_control.xacro` | `hardware/src/diffbot_system.cpp` + `firmware/firmware.ino` (ESP32) |
| Лидар | плагин `sensors/laser.urdf.xacro` | `sllidar_ros2/` (`sllidar_node`) |
| Запуск управляющего слоя | `simulator/launch/gazebo.launch.py` | `bringup/launch/diffbot_system.launch.py` |
| Время | `/clock` от Gazebo | системное время |

Причём все слои выше ориентируются на одни и тот же интерфейс (`/scan`, `/tf`, `/cmd_vel`),
поэтому навигация работает поверх любого варианта, и то, что отлажено в симуляции,
переносится на реального робота без переписывания.

## Откуда рождаются узлы (источники в коде)

| Место | Что там |
|---|---|
| `*/launch/*.py` (`Node(...)`) | Управление запуском, список полного стека |
| `description/**/*.xacro` (Gazebo-плагины) | «Скрытые» узлы симуляции: diff_drive, лидар, камеры, IMU |
| `CMakeLists.txt` (`install(TARGETS)`) | Исполняемые C++ (например, `rf2o_laser_odometry_node`) |
| `setup.py` (`console_scripts`) | Python-узлы (например, `mecanum_control`, `serial_com`) |

## Неочевидное (ловушки, экономящие время потом)

1. **`/joint_states` в симуляции пуст.** diff_drive крутит колёса внутри физики, а
   `joint_state_publisher` не включён в `gazebo.launch.py`. На реальном роботе углы
   придут от прошивки ESP32 (micro-ROS) — анатом «оживёт» отдельно от колёс.
2. **Гонка при первом spawn.** `spawn_entity` может упасть, если не дождался сервисов Gazebo
   (мир грузится >30 с). Повторный запуск проходит успешно:
   `ros2 run gazebo_ros spawn_entity.py -topic robot_description -entity my_bot`.
3. **README.md**: команда `ros2 launch robolaunch_cloudy_simulator launch_sim.launch.py`
   НЕ работает — такого launch-файла нет. Правильный: `gazebo.launch.py` (аргументы `world:=playground|warehouse`).
4. **SDF-версии миров** в `simulator/worlds/` должны соответствовать версии Gazebo: с v11.14
   увидишь предупреждения `Unable to convert from SDF version 1.9` — мир может грузиться пустым
   (в репо исправлено на 1.7).
5. **Аргумент `vehicle`** в launch-файлах (`cloudy_v2` | `cloudy_v1` | `arcelik`) выбирает модель
   из `description/` — меняет всю нижнюю систему (колёса, привод).

## Быстрые команды

```bash
# Симуляция
ros2 launch robolaunch_cloudy_simulator gazebo.launch.py          # мир=warehouse (по умолчанию)
ros2 launch robolaunch_cloudy_simulator gazebo.launch.py world:=playground

# Управление
ros2 run teleop_twist_keyboard teleop_twist_keyboard

# SLAM + навигация в симе
ros2 launch robolaunch_cloudy_navigation sim_launch_all.launch.py

# Осмотр
ros2 node list && ros2 topic list && rqt_graph

# Реальный робот (когда будет железо)
ros2 launch robolaunch_cloudy_bringup diffbot_system.launch.py
```
