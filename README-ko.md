# Node VM detect
Nodejs에서 PC가 VM환경인지 감지하는 라이브러리 입니다.  
[VMAware](https://github.com/kernelwernel/VMAware) 라이브러리를 통해 C++기반 크로스 플랫폼으로 동작합니다.  
electron 기반 앱에서 VM을 감지하는데 유용하게 사용할 수 있을걸로 기대합니다.  

## 설치 및 실행
```
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

VM감지 및 각 항목에 대한 자세한 사항은 [VMAware](https://github.com/kernelwernel/VMAware)를 참고하세요.

## 동작 플랫폼 & 요구사항
VMAware는 32bit를 지원하나, 이 라이브러리는 64bit만 지원합니다.  
* Node 18(electron 23) 이상이 필요합니다.

| OS      | Arch  | 동작여부 |
| ------- | ----- | -------- |
| Windows | x64   | ✅        |
| Windows | x86   | ❌        |
| Windows | Arm64 | ❌        |
| Windows | Arm32 | ❌        |
| MacOS   | x64   | ✅        |
| MacOS   | Arm64 | ✅        |
| Linux   | x64   | ✅        |
| Linux   | x86   | ❌        |
| Linux   | Arm64 | ✅        |
| Linux   | Arm32 | ❌        |

## TODO
windows Arm arch빌드에 문제가 있어 확인 중입니다.  
VMAware의 flag system등 유용한 기능을 구현할 예정입니다.

