# Node VM detect
Nodejs에서 PC가 VM환경인지 감지하는 라이브러리 입니다.  
[VMAware](https://github.com/kernelwernel/VMAware) 라이브러리를 통해 C++기반 크로스 플랫폼으로 동작합니다.  
electron 기반 앱에서 VM을 감지하는데 유용하게 사용할 수 있을걸로 기대합니다.  
VM감지에 대한 자세한 사항은 [VMAware](https://github.com/kernelwernel/VMAware)를 참고하세요.

# 설치
```
npm i node-vm-detect
```

# 사용
## 기본
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

### 기본 preset 지정
`DEFAULT`, `ALL` 중 하나만 선택해서 `preset` 으로 지정합니다.  
| Flag      | Desciption                                                |
| --------- | --------------------------------------------------------- |
| `DEFAULT` | 제외된 technique 외 모든 기술을 사용합니다. 기본값입니다. |
| `ALL`     | 제외된 technique을 포함하여 모든 기술을 사용합니다.       |
```
preset: 'DEFAULT' | 'ALL'
```
### 그외 setting flags 지정
그외 setting flags는 `settings`에 배열 형태로 지정합니다.
| Flag             | Desciption                                                               |
| ---------------- | ------------------------------------------------------------------------ |
| `NO_MEMO`        | 메모이제이션을 비활성화 하여, 이전 검사 결과를 무시하고 다시 검사합니다. |
| `MULTIPE`        | `brand`에만 적용됩니다. 복수개의 VM Host를 표시합니다.                   |
| `HIGH_THRESHOLD` | `isVM`과 `percentage`에만 적용됩니다. VM여부 판별의 임계값을 높입니다.   |
| `DYNAMIC`        | `conclusion`에만 적용됩니다. conclusion 메시지를 상세히 표시합니다.      |
```
settings: ('NO_MEMO' | 'MULTIPLE' | 'HIGH_THRESHOLD' | 'DYNAMIC')[]
```

## Technique flags
VMAware의 [flag table](https://github.com/kernelwernel/VMAware/wiki/Documentation#flag-table) 에 대응됩니다.  
특정 technique을 제외하거나, 특정 technique만을 적용할 수 있습니다.
```
techniques: {
  only: TechniqueFlags[],
  disable: TechniqueFlags[]
}
```

## 예시
### 1. 제외없이 모든 technique사용
```js
getVMInfo({
  preset: 'ALL',
});
```
### 2. 메모이제이션을 사용하지 않고 감지 임계값을 높임
```js
getVMInfo({
  settings: ['NO_MEMO', 'HIGH_THRESHOLD'],
});
```
### 3. VMID technique을 사용하지 않음
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

## 기타
각 요소만 얻는 함수들.
```js
import {
  getBrands,
  getType,
  getConclusion,
  getIsVM,
  getPercentage,
  getDetectedTechniques,
} from 'node-vm-detect';
```
default import로도 사용할 수 있습니다.
```js
import nodeVMDetect from 'node-vm-detect';

nodeVMDetect.info();
nodeVMDetect.isVM();
nodeVMDetect.brand();
nodeVMDetect.type();
nodeVMDetect.conclusion();
nodeVMDetect.percentage();
nodeVMDetect.detectedTechniques();
```

# 동작 플랫폼 & 요구사항
VMAware는 32bit를 지원하나, 이 라이브러리는 64bit만 지원합니다.  
* Node 18(electron 23) 이상이 필요합니다.

| OS      | Arch  | 동작여부 |
| ------- | ----- | -------- |
| Windows | x64   | ✅        |
| Windows | x86   | ❌        |
| Windows | Arm64 | ✅        |
| Windows | Arm32 | ❌        |
| MacOS   | x64   | ✅        |
| MacOS   | Arm64 | ✅        |
| Linux   | x64   | ✅        |
| Linux   | x86   | ❌        |
| Linux   | Arm64 | ✅        |
| Linux   | Arm32 | ❌        |

# TODOS
* Deno FFI를 통한 Deno 지원
* 다른 VM 감지 패키지 include

# LICENSE
MIT
