# Node VM Detect

A Node.js library for detecting whether the current system is running in a virtual machine (VM).    
It is powered by the cross-platform C++ library [VMAware](https://github.com/kernelwernel/VMAware).  
This can be particularly useful for detecting VM environments in Electron-based applications.  
For more detailed information on VM detection, refer to [VMAware](https://github.com/kernelwernel/VMAware).

# Install

```bash
npm i node-vm-detect
```

# Usage
Simply call it without any options—this is sufficient for most use cases.
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
| Value                | Type             | Description                             |
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

### Set the setting flags
Setting flags can be specified as an array in the `settings` field.
```
settings: ('MULTIPLE' | 'HIGH_THRESHOLD' | 'DYNAMIC')[]
```

| Flag             | Description                               |
| ---------------- | ----------------------------------------- |
| `MULTIPE`        | Displays multiple VM Hosts.               |
| `HIGH_THRESHOLD` | Increases the threshold for VM detection. |
| `DYNAMIC`        | Display the conclusion message in detail. |


## Technique flags
These correspond to VMAware’s [flag table](https://github.com/kernelwernel/VMAware/wiki/Documentation#flag-table).  

### Setting a preset
`techniques` can be selected from `DEFAULT` or `ALL`.  
```
techniques: 'DEFAULT' | 'ALL'
```

| Flag      | Description                                                       |
| --------- | ----------------------------------------------------------------- |
| `DEFAULT` | Use all techniques except the excluded ones. This is the default. |
| `ALL`     | Use all techniques, including excluded techniques.                |

### Custom technique flags
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
  techniques: 'ALL',
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

## Detection Module
By default, the detection module (.node) is automatically located based on the installed path `node_modules/node-vm-detect`.  
### Extracting Modules
However, in cases where the module needs to be used from a separate path for various reasons (e.g., adding code signing, not loading libraries from node_modules, etc.), you can extract the .node files from node_modules.
```shell
npx node-vm-detect extract -d {{destination}} -p {{platform}} -a {{arch}}
```
### Example
Extract only x86_64 binaries for Windows and Linux into the /node-vm-detect directory of the current project:
```shell
npx node-vm-detect extract -d node-vm-detect -p win32 linux -a x64
```
### Custom Path
You can specify a custom path for the extracted modules using the `moduleRoot`option.  
In this case, the module files must exist under the appropriate `moduleRoot/platform/arch` directory structure.
#### Example
In Electron, specify `/node-vm-detect` under the app path:
```ts
import path from 'path';
import {app} from 'electron';

getVMInfo({
  moduleRoot: path.resolve(app.getAppPath(), 'node-vm-detect'),
})
```

## Child Process
By default, VM detection runs in the `current process`.  
If you want to distribute load or run each detection independently without caching results, use the `runOnChild` option to execute in a child process.  
When runOnChild is enabled, a child Node process is spawned via fork.
```ts
getVMInfo({
  runOnChild: true,
});
```
### Extracting Child Process Module & Custom Path
Like the detection module, the child process module is automatically located based on node_modules/node-vm-detect, and can also be custom-extracted and configured.  
When extracted, the module file name is fixed as `node-vm-dectect-child.js`.
```shell
npx node-vm-detect extract -d {{destination}} -c
```
#### Extraction Example
Extract to the /node-vm-detect directory in the current project:
```shell
npx node-vm-detect extract -d node-vm-detect -c
```
#### Custom Path
In Electron, specify /node-vm-detect/node-vm-detect-child.js under the app path:
```ts
import path from 'path';
import {app} from 'electron';

getVMInfo({
  runOnChild: path.resolve(app.getAppPath(), 'node-vm-detect', 'node-vm-detect-child.js'),
});
```

## Administrator Privileges
Some VM detection techniques require administrator privileges.  
node-vm-detect does not request elevated privileges on its own, so run Node with administrator privileges when necessary.  
* In Electron, it is recommended not to run Electron itself with administrator privileges. Instead, run Electron with normal privileges and execute a separate process with administrator privileges specifically for detection.


# Supported Platforms & Requirements
Supports Linux, macOS, and Windows. Requires `Node.js 18 (Electron 23) or later`. 
* VMAware supports 32-bit, but this library only supports 64-bit platforms. 

# TODOS
* Deno support with Deno FFI
* Include other VM detection packages

# LICENSE
MIT
