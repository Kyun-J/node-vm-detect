# Node VM Detect

A Node.js library that detects whether the current environment is a virtual machine (VM).  
It is built on top of the [VMAware](https://github.com/kernelwernel/VMAware) library and operates as a C++ based crossplatform module.  
This is particularly useful for Electron-based applications that need to detect VM environments.

## Installation & Usage

```bash
npm i node-vm-detect
```

```js
import { getVMInfo } from 'node-vm-detect';

const {
  isVM,
  brand,
  type,
  conclusion,
  percentage,
  detectedCount,
  techniqueCount,
} = await getVMInfo();
```
For detailed information on VM detection and each related item, please refer to [VMAware](https://github.com/kernelwernel/VMAware).

## Supported Platforms & Requirements

VMAware supports 32-bit environments, but this library only supports 64-bit platforms.  
* Requires Node.js 18 or later (Electron 23 or later).

| OS      | Arch  | Supported |
| ------- | ----- | --------- |
| Windows | x64   | ✅         |
| Windows | x86   | ❌         |
| Windows | Arm64 | ❌         |
| Windows | Arm32 | ❌         |
| macOS   | x64   | ✅         |
| macOS   | Arm64 | ✅         |
| Linux   | x64   | ✅         |
| Linux   | x86   | ❌         |
| Linux   | Arm64 | ✅         |
| Linux   | Arm32 | ❌         |

## TODO

There is currently an issue with building for Windows ARM architectures, which is under investigation.  
Future plans include implementing useful features from VMAware such as its flag system.
