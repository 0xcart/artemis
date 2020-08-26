# artemis

Visual Studio Code development environment for SparkFun Artemis based boards targeting Microsoft Windows 10.

## Dependencies

The latest version of each tool/extension should be used unless otherwise noted. During installation, if the option is available to add a given tool to the PATH environment variable, please do so. This is very important for ARM GCC.

The following tools are required to make use of this repository:

* [git](<https://git-scm.com/>)
* [ARM GCC](<https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm>)
* [Segger J-Link](<https://www.segger.com/downloads/jlink/>)
* [Visual Studio Code](<https://code.visualstudio.com/>)

The following Visual Studio Code extensions are required:

* [Cortex-Debug](<https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug>)
* [C/C++](<https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools>)

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

Click `File`, `Open Folder...` from the menu. Select the `artemis` folder and click `Select Folder`. You should now see the following in the Explorer view within Visual Studio Code:

![Explorer](doc/image/explorer.jpg)

### Configure Visual Studio Code extensions
