# Node VM Detect

A Node.js library that detects whether the current environment is a virtual machine (VM).   
It operates as a cross-platform C++ module via the [VMAware](https://github.com/kernelwernel/VMAware) library.  
This can be particularly useful for detecting VM environments in Electron-based applications.  
For more detailed information on VM detection, refer to [VMAware](https://github.com/kernelwernel/VMAware).

# Install

```bash
npm i node-vm-detect
```

# Usage
## Basic
Call without any options—this is sufficient for most use cases.
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
## Setting flags
These correspond to the [setting flags](https://github.com/kernelwernel/VMAware/wiki/Documentation#setting-flags) used in VMAware.  
`Presets` can be selected from `DEFAULT` or `ALL` (only one must be selected).  
Other flags can be specified as an array in the `settings` field.
### Setting a preset
```
preset: 'DEFAULT' | 'ALL'
```
### Additional setting flags
```
settings: ('NO_MEMO' | 'MULTIPLE' | 'HIGH_THRESHOLD' | 'DYNAMIC')[]
```

## Technique flags
These correspond to VMAware’s [flag table](https://github.com/kernelwernel/VMAware/wiki/Documentation#flag-table).  
You can selectively exclude or include certain techniques.  
```
techniques: {
  only: TechniqueFlags[],
  disable: TechniqueFlags[]
}
```

## Examples
### 1. Use all available techniques without exclusions
```js
getVMInfo({
  preset: 'ALL',
});
```
### 2. Disable memoization and increase detection threshold
```js
getVMInfo({
  settings: ['NO_MEMO', 'HIGH_THRESHOLD'],
});
```
### 3. Exclude the `VMID` technique
```js
getVMInfo({
  techniques: {
    disable: ['VMID'],
  },
});
```
### 4. Use only the `CPU_BRAND` technique
```js
getVMInfo({
  techniques: {
    only: ['CPU_BRAND'],
  },
});
```
Same result as:
```js
getVMInfo({
  techniques: {
    only: ['VMID', 'CPU_BRAND'],
    disable: ['VMID'],
  },
});
```

## Utilities
Functions to retrieve individual values:
```js
import {
  getBrands,
  getType,
  getConclusion,
  getIsVM,
  getPercentage,
  getDetectedCount,
} from 'node-vm-detect';
```
Also available via default import:
```js
import nodeVMDetect from 'node-vm-detect';

nodeVMDetect.info();
nodeVMDetect.isVM();
nodeVMDetect.brand();
nodeVMDetect.type();
nodeVMDetect.conclusion();
nodeVMDetect.percentage();
nodeVMDetect.detectedCount();
```

# Supported Platforms & Requirements
VMAware supports 32-bit environments, but this library only supports 64-bit platforms.  
* Requires Node.js 18 or later (Electron 23 or later).

| OS      | Arch  | Supported |
| ------- | ----- | --------- |
| Windows | x64   | ✅         |
| Windows | x86   | ❌         |
| Windows | Arm64 | ❌ (TODO)  |
| Windows | Arm32 | ❌         |
| MacOS   | x64   | ✅         |
| MacOS   | Arm64 | ✅         |
| Linux   | x64   | ✅         |
| Linux   | x86   | ❌         |
| Linux   | Arm64 | ✅         |
| Linux   | Arm32 | ❌         |

# TODO
There is currently an issue with building for Windows ARM architectures, which is under investigation. 

# LICENSE
MIT
