# entri_ros2_pubsub_cpp

A ROS2 C++ package that defines a custom message, `RobotStatus`, and implements a publisher and a subscriber for it. It is the communication backbone of a simple smart-factory robot monitor.

- **Build system:** `ament_cmake`
- **Language:** C++
- **Tested on:** ROS2 Jazzy, Ubuntu 24.04 (WSL2). The `CMakeLists.txt` also falls back to the older `rosidl_target_interfaces` call, so it builds on Humble too.

## How it works

| Part | File | What it does |
|------|------|--------------|
| Custom message | `msg/RobotStatus.msg` | `int32 robot_id`, `float32 battery_level`, `string status_message` |
| Publisher | `src/publisher_node.cpp` | Node `minimal_publisher`. Publishes on `robot_status` every 500 ms with `robot_id = 12` and `status_message = "Operating"`. The battery starts at 100.0 and drops by 0.5 each tick. |
| Subscriber | `src/subscriber_node.cpp` | Node `minimal_subscriber`. Subscribes to `robot_status` and logs `Robot X is at Y% battery. Status: Z` |
| Package metadata | `package.xml` | `rclcpp`, `rosidl_default_generators`, `rosidl_default_runtime`, `std_msgs` (build and exec dependencies) |
| Build config | `CMakeLists.txt` | Generates the interface, builds both executables and links them to the generated message type |

## Project layout

```
entri_ros2_pubsub_cpp/
├── CMakeLists.txt
├── package.xml
├── README.md
├── msg/
│   └── RobotStatus.msg
└── src/
    ├── publisher_node.cpp
    └── subscriber_node.cpp
```

## Requirements

- Ubuntu 24.04 with ROS2 Jazzy (`ros-jazzy-ros-base` or `ros-jazzy-desktop`)
- `colcon` (`sudo apt install python3-colcon-common-extensions`)
- A C++ compiler (`sudo apt install build-essential`)

## Build

The package folder must be inside the `src` folder of a ROS2 workspace.

```bash
source /opt/ros/jazzy/setup.bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
git clone <this-repo-url> entri_ros2_pubsub_cpp
cd ~/ros2_ws
colcon build --packages-select entri_ros2_pubsub_cpp
```

A successful build ends with `Finished <<< entri_ros2_pubsub_cpp`.

## Run

Use two terminals. Source the workspace in each one.

**Terminal 1: subscriber**

```bash
cd ~/ros2_ws
source install/setup.bash
ros2 run entri_ros2_pubsub_cpp subscriber_node
```

**Terminal 2: publisher**

```bash
cd ~/ros2_ws
source install/setup.bash
ros2 run entri_ros2_pubsub_cpp publisher_node
```

Stop either node with `Ctrl+C`.

### Expected output

Subscriber terminal, one line every 0.5 s:

```
[INFO] [...] [minimal_subscriber]: Robot 12 is at 100.0% battery. Status: Operating
[INFO] [...] [minimal_subscriber]: Robot 12 is at 99.5% battery. Status: Operating
[INFO] [...] [minimal_subscriber]: Robot 12 is at 99.0% battery. Status: Operating
```

Publisher terminal:

```
[INFO] [...] [minimal_publisher]: Publishing status for Robot 12
```

## How I tested it

1. **Build check.** `colcon build --packages-select entri_ros2_pubsub_cpp` completed with no errors.
2. **Interface check.** The custom message was generated and registered:
   ```bash
   source install/setup.bash
   ros2 interface show entri_ros2_pubsub_cpp/msg/RobotStatus
   ```
   It shows `robot_id`, `battery_level` and `status_message`.
3. **Topic check.** With the publisher running:
   ```bash
   ros2 topic list                          # lists /robot_status
   ros2 topic echo /robot_status --once     # prints one message with robot_id: 12
   ```
4. **End-to-end check.** I ran the subscriber and publisher in separate terminals and confirmed the subscriber printed `Robot 12 is at <battery>% battery. Status: Operating`, with the battery dropping by 0.5 each message.
5. **Log-format check.** This mimics the automated grader:
   ```bash
   ros2 run entri_ros2_pubsub_cpp subscriber_node > /tmp/sub.log 2>&1 &
   SUB=$!
   sleep 2
   timeout 5 ros2 run entri_ros2_pubsub_cpp publisher_node > /dev/null 2>&1
   kill $SUB
   grep -E "Robot [0-9]+ is at [0-9.]+% battery" /tmp/sub.log && echo PASS || echo FAIL
   ```

## Troubleshooting

| Problem | Fix |
|---------|-----|
| `Package 'entri_ros2_pubsub_cpp' not found` | Run `source install/setup.bash` in that terminal. |
| `package.xml` error about redundant dependencies | Declare each dependency only as `build_depend` and `exec_depend`, not also as a generic `<depend>`. |
| `robot_status.hpp` not found | Check that the message file is at `msg/RobotStatus.msg` and listed in `rosidl_generate_interfaces`. |
| Subscriber prints nothing | Start the subscriber first, and make sure both terminals use the same ROS distro and `ROS_DOMAIN_ID`. |
| Stale build errors | `rm -rf build install log` in the workspace, then rebuild. |

## License

Apache-2.0
