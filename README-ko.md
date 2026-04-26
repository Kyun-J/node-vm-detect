# Node VM detect
Nodejs에서 PC가 VM환경인지 감지하는 라이브러리 입니다.  
[VMAware](https://github.com/kernelwernel/VMAware) 라이브러리를 통해 C++기반 크로스 플랫폼으로 동작합니다.  
electron 기반 앱에서 VM을 감지하는데 유용하게 사용할 수 있을 것으로 기대합니다.  
VM감지에 대한 자세한 사항은 [VMAware](https://github.com/kernelwernel/VMAware)를 참고하세요.

# 설치
```
npm i node-vm-detect
```

# 사용
아무런 option 없이 호출합니다. 대부분의 경우엔 이것만으로 충분합니다.
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
| Value                | Type             | Desciption                   |
| -------------------- | ---------------- | ---------------------------- |
| `isVM`               | boolean          | 현재 PC가 VM인지 여부        |
| `brand`              | string           | VM 브랜드                    |
| `type`               | string           | VM 유형                      |
| `conclusion`         | string           | 감지 결과에 대한 해설        |
| `percentage`         | number           | VM 가능성. 0 - 100 사이의 값 |
| `detectedTechniques` | TechniqueFlags[] | 감지된 technique목록         |

## Setting flags
VMAware의 [setting flags](https://github.com/kernelwernel/VMAware/wiki/Documentation#setting-flags)에 대응됩니다.  
각 플래그에 대한 자세한 설명은 VMAware를 참조하세요.

### setting flags 지정
setting flags는 `settings`에 배열 형태로 지정합니다.
```
settings: ('MULTIPLE' | 'HIGH_THRESHOLD' | 'DYNAMIC')[]
```

| Flag             | Desciption                             |
| ---------------- | -------------------------------------- |
| `MULTIPE`        | 복수개의 VM Host를 표시합니다.         |
| `HIGH_THRESHOLD` | VM여부 판별의 임계값을 높입니다.       |
| `DYNAMIC`        | conclusion 메시지를 상세히 표시합니다. |

## Technique flags

VMAware의 [flag table](https://github.com/kernelwernel/VMAware/wiki/Documentation#flag-table) 에 대응됩니다.  

### 기본 preset 지정
`DEFAULT`, `ALL` 프리셋 중 하나를 선택해서 지정할 수 있습니다.
```
techniques: 'DEFAULT' | 'ALL'
```

| Flag      | Desciption                                                |
| --------- | --------------------------------------------------------- |
| `DEFAULT` | 제외된 technique 외 모든 기법을 사용합니다. 기본값입니다. |
| `ALL`     | 제외된 technique을 포함하여 모든 기법을 사용합니다.       |

### Techiques 지정
특정 technique을 제외하거나, 특정 technique만을 적용할 수 있습니다.
```
techniques: {
  only: TechniqueFlags[],
  disable: TechniqueFlags[]
}
```

### 예시
#### 1. 제외없이 모든 technique사용
```js
getVMInfo({
  techniques: 'ALL',
});
```
#### 2. 감지 임계값을 높이고 conclusion 메시지 상세히 표시.
```js
getVMInfo({
  settings: ['HIGH_THRESHOLD', 'DYNAMIC'],
});
```
#### 3. VMID technique을 사용하지 않음
```js
getVMInfo({
  techniques: {
    disable: ['VMID'],
  },
});
```
### 4. CPU_BRAND technique만 사용
```js
getVMInfo({
  techniques: {
    only: ['CPU_BRAND'],
  },
});
```
위와 동일한 결과
```js
getVMInfo({
  techniques: {
    only: ['VMID', 'CPU_BRAND'],
    disable: ['VMID'],
  },
});
```

## 감지 모듈
기본적으로 설치된 node_modules/node-vm-detect 경로를 기반으로 감지 모듈(.node)을 자동으로 찾습니다.  
### 모듈 추출
하지만 여러가지 이유로 모듈을 별도의 경로에서 사용해야 하는 경우(ex. 코드사이닝 추가, node_modules에서 라이브러리를 불러오지 않음 등)를 대비하여 node_moduels에서 .node파일을 추출할 수 있습니다.
```shell
npx node-vm-detect extract -d {{destination}} -p {{platform}} -a {{arch}}
```
### 예시
현재 프로젝트의 /node-vm-detect 경로에 windows, linux의 x86_64만 추출
```shell
npx node-vm-detect extract -d node-vm-detect -p win32 linux -a x64
```
### 경로 지정
`moduleRoot` 옵션으로 추출한 모듈의 경로를 커스텀하게 지정할 수 있습니다.  
이때, 모듈 파일이 각 `moduleRoot/platform/arch` 경로에 알맞게 존재하여야 합니다.
#### 예시
electron에서, 앱 경로의 /node-vm-detect 을 지정
```ts
import path from 'path';
import {app} from 'electron';

getVMInfo({
  moduleRoot: path.resolve(app.getAppPath(), 'node-vm-detect'),
})
```

## 자식 프로세스
VM감지는 기본적으로 `현재 프로세스` 에서 동작합니다.  
부하 분산 및 감지 결과를 캐싱하지 않고 매 감지마다 독립적으로 사용하고자 하면, 자식 프로세스에서 동작하는 `runOnChild` 옵션을 사용하세요.  
runOnChild사용 시 자식 노드 프로세스를 fork로 실행합니다.
```ts
getVMInfo({
  runOnChild: true,
});
```
### 자식 프로세스 모듈 추출 및 경로 지정
감지 모듈과 마찬가지로 자식 프로세스 모듈도 node_modules/node-vm-detect 경로를 기반으로 자동으로 찾으며, 커스텀하게 추출 및 경로 지정을 할 수 있습니다.  
추출 시 모듈 파일 명은 `node-vm-dectect-child.js` 로 고정됩니다.
```shell
npx node-vm-detect extract -d {{destination}} -c
```
#### 추출 예시
현재 프로젝트의 /node-vm-detect 경로에 추출.
```shell
npx node-vm-detect extract -d node-vm-detect -c
```
#### 경로 지정 예시
electron에서 앱 경로의 /node-vm-detect/node-vm-detect-child.js 을 지정
```ts
import path from 'path';
import {app} from 'electron';

getVMInfo({
  runOnChild: path.resolve(app.getAppPath(), 'node-vm-detect', 'node-vm-detect-child.js'),
});
```

## 관리자 권한
일부 VM감지 기법들은 관리자 권한을 요구합니다.  
node-vm-detect는 별도로 관리자 권한을 요청하거나 하지 않으며, 필요시 node를 관리자 권한으로 실행하여 사용하세요.  
* electron의 경우 electron 자체를 관리자로 실행하기 보단, electron은 일반 권한으로 실행하고 감지를 위한 별도의 프로세스를 관리자 권한으로 실행하여 사용하는 것을 권장합니다.

# 동작 플랫폼 & 요구사항
linux, MacOS, Windows를 지원하며, `Node 18(electron 23)` 이상이 필요합니다.
* VMAware는 32bit를 지원하나, 이 라이브러리는 64bit만 지원합니다.  

# LICENSE
MIT
