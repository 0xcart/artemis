# artemis

Visual Studio Code development environment for [SparkFun Artemis](https://www.sparkfun.com/artemis) based boards.

## Dependencies

The latest version of each tool/extension should be used unless otherwise noted. During installation, if the option is available to add a given tool to the PATH environment variable, please do so. This is very important for ARM GCC.

The following tools are required to make use of this repository:

* [git](<https://git-scm.com/>)
* [GNU Arm Embedded Toolchain](<https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm>)
* [Segger J-Link](<https://www.segger.com/downloads/jlink/>)
* [Visual Studio Code](<https://code.visualstudio.com/>)

The following Visual Studio Code extensions are required:

* [C/C++](<https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools>)
* [Cortex-Debug](<https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug>)

The following Visual Studio Code extensions are recommended:

* [ARM](<https://marketplace.visualstudio.com/items?itemName=dan-c-underwood.arm>)
* [LinkerScript](<https://marketplace.visualstudio.com/items?itemName=ZixuanWang.linkerscript>)

## Getting Started

### Clone this repository and initialize its submodules

```shell
  git clone https://github.com/0xcart/artemis.git
  cd artemis
  git submodule update --init --recursive
```

  This repository includes the SparkFun copy of the [AmbiqSuite SDK](https://github.com/sparkfun/AmbiqSuiteSDK) as a git submodule. In turn, this copy of the SDK includes the SparkFun [Board Support Package](https://github.com/sparkfun/SparkFun_Apollo3_AmbiqSuite_BSPs) repository as a git submodule. By using the --init --recursive flags both submodules will be initialized and updated correctly.

### Open the project

Launch Visual Studio Code.

Click `File`, `Open Folder...` from the menu. Select the `artemis` folder and click `Select Folder`. You should now see the following in the `Explorer` view within Visual Studio Code:

![Explorer](doc/image/explorer.jpg)

### Configure the Cortex-Debug Visual Studio Code extension

Highlight the Cortex-Debug extension in the `Extensions` view within Visual Studio Code. Click the `Manage` (gear) icon. Click the `Extension Settings` option in the displayed popup menu. A `Settings` document will be loaded for the extension.

Click on `Edit in settings.json` which will open the `settings.json` document. Update the settings with the following information taking care the paths for the J-Link GDB server and ARM toolchain are correct for your system configuration.

```json
{
  "editor.tabSize": 2,
  "editor.wordWrapColumn": 120,
  "cortex-debug.JLinkGDBServerPath": "C:/Program Files (x86)/SEGGER/JLink/JLinkGDBServerCL.exe",
  "cortex-debug.armToolchainPath": "C:/Program Files (x86)/GNU Arm Embedded Toolchain/9 2020-q2-update/bin",
  "window.zoomLevel": 0,
}
```

### Update c_cpp_properties.json settings in the .vscode folder

Update the `"compilerPath"` value to match your system configuration.

### Update launch.json settings in the .vscode folder

Update the `"serverpath"` value to match your system configuration. Also update the `"cpuFrequency"`, `"swoFrequency"`, and `"device"` values to match your Artemis board.

## Build, Run, and Debug

