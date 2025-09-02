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
Call without any options—this is sufficient for most use cases.
```js
import { getVMInfo } from 'node-vm-detect';

const {
  isVM,
  brand,
  type,
  conclusion,
  percentage,
  detectedTechniques,
} = await getVMInfo();
```
## Return value
| Value                | Type             | Desciption                              |
| -------------------- | ---------------- | --------------------------------------- |
| `isVM`               | boolean          | Whether the current PC is a VM          |
| `brand`              | string           | VM brand                                |
| `type`               | string           | VM type                                 |
| `conclusion`         | string           | Commentary on detection results         |
| `percentage`         | number           | VM certainty. A value between 0 and 100 |
| `detectedTechniques` | TechniqueFlags[] | List of detected techniques             |

## Setting flags
These correspond to the [setting flags](https://github.com/kernelwernel/VMAware/wiki/Documentation#setting-flags) used in VMAware.  
For a detailed description of each flag, refer to VMAware.

### Setting a preset
`Presets` can be selected from `DEFAULT` or `ALL` (only one must be selected).  
```
preset: 'DEFAULT' | 'ALL'
```

| Flag      | Desciption                                                        |
| --------- | ----------------------------------------------------------------- |
| `DEFAULT` | Use all techniques except the excluded ones. This is the default. |
| `ALL`     | Use all techniques, including excluded techniques.                |

### Additional setting flags
Additional setting flags can be specified as an array in the `settings` field.
```
settings: ('MULTIPLE' | 'HIGH_THRESHOLD' | 'DYNAMIC')[]
```

| Flag             | Desciption                                                 |
| ---------------- | ---------------------------------------------------------- |
| `MULTIPE`        | Displays multiple VM Hosts.                                |
| `HIGH_THRESHOLD` | Increase the threshold for determining if a VM is present. |
| `DYNAMIC`        | Display the conclusion message in detail.                  |


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
### 2. Increase detection threshold and show more detail in the conclusion message
```js
getVMInfo({
  settings: ['HIGH_THRESHOLD', 'DYNAMIC'],
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

## Clean
VM detection is performed on the child process.  
After detection, child process is removed.
This is useful for new detections without affecting existing results, or to reduce memory usage.  
When called with `false`, it works in the current process, and the detection results are stored in memory and recycled on each call with `false`.  
The default value is `true`.  
```js
getVMInfo({ clean: true });
```


# Supported Platforms & Requirements
VMAware supports 32-bit environments, but this library only supports 64-bit platforms.  
* Requires Node.js 18 or later (Electron 23 or later).

| OS      | Arch  | Supported |
| ------- | ----- | --------- |
| Windows | x64   | ✅         |
| Windows | x86   | ❌         |
| Windows | Arm64 | ✅         |
| Windows | Arm32 | ❌         |
| MacOS   | x64   | ✅         |
| MacOS   | Arm64 | ✅         |
| Linux   | x64   | ✅         |
| Linux   | x86   | ❌         |
| Linux   | Arm64 | ✅         |
| Linux   | Arm32 | ❌         |

# TODOS
* Deno support with Deno FFI
* Include other VM detection packages

# LICENSE
MIT
