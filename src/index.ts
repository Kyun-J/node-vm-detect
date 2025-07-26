export interface VMInfo {
  isVM: boolean;
  brand: string;
  type: string;
  conclusion: string;
  percentage: number;
  detectedCount: number;
  techniqueCount: number;
}

let napi: {
  getVMInfo: () => Promise<VMInfo>;
};

const getNapi = async () => {
  if (napi) {
    return napi;
  }

  let napiPath: string | undefined;
  if (process.platform === 'win32') {
    if (process.arch === 'x64') {
      napiPath = './win/x64/detectWindows.node';
    } else if (process.arch === 'arm64') {
      napiPath = './win/arm64/detectWindows.node';
    }
  } else if (process.platform === 'darwin') {
    if (process.arch === 'arm64') {
      napiPath = './mac/arm64/detectMac.node';
    } else if (process.arch === 'x64') {
      napiPath = './mac/x64/detectMac.node';
    }
  }
  if (!napiPath) {
    throw new Error('Unsupported platform');
  }

  napi = await import(napiPath);
  return napi;
};

export const getVMInfo = async (): Promise<VMInfo> => {
  const napi = await getNapi();
  return napi.getVMInfo();
};
