import { createRequire } from 'node:module';
import type {
  SettingFlags,
  PresetFlags,
  TechniqueFlags,
  PercentageSettings,
  ConclusionSettings,
  BrandSettings,
  TypeSettings,
  IsVMSettings,
  DetectedTechniquesSettings,
} from './flags';

export interface VMInfo {
  isVM: boolean;
  brand: string;
  type: string;
  conclusion: string;
  percentage: number;
  detectedCount: number;
  detectedTechniques: TechniqueFlags[];
}

export interface VMInfoOptions<T extends SettingFlags[] = SettingFlags[]> {
  preset?: PresetFlags;
  settings?: T;
  techniques?: {
    only?: TechniqueFlags[];
    disable?: TechniqueFlags[];
  };
}

const nodeVMDetect: {
  info: (options?: VMInfoOptions) => Promise<VMInfo>;
  isVM: (options?: VMInfoOptions<IsVMSettings>) => Promise<boolean>;
  brand: (options?: VMInfoOptions<BrandSettings>) => Promise<string>;
  type: (options?: VMInfoOptions<TypeSettings>) => Promise<string>;
  conclusion: (options?: VMInfoOptions<ConclusionSettings>) => Promise<string>;
  percentage: (options?: VMInfoOptions<PercentageSettings>) => Promise<number>;
  detectedTechniques: (
    options?: VMInfoOptions<DetectedTechniquesSettings>,
  ) => Promise<TechniqueFlags[]>;
} = (() => {
  const { platform, arch } = process;
  if (
    !['darwin', 'win32', 'linux'].includes(platform) ||
    !['x64', 'arm64'].includes(arch)
  ) {
    throw new Error('Unsupported platform');
  }
  return createRequire(import.meta.url)(`./${platform}/${arch}/vmDetect.node`);
})();

export default nodeVMDetect;

export const getVMInfo = (options?: VMInfoOptions): Promise<VMInfo> =>
  nodeVMDetect.info(options);

export const getIsVM = (
  options?: VMInfoOptions<IsVMSettings>,
): Promise<boolean> => nodeVMDetect.isVM(options);

export const getBrand = (
  options?: VMInfoOptions<BrandSettings>,
): Promise<string> => nodeVMDetect.brand(options);

export const getType = (
  options?: VMInfoOptions<TypeSettings>,
): Promise<string> => nodeVMDetect.type(options);

export const getConclusion = (
  options?: VMInfoOptions<ConclusionSettings>,
): Promise<string> => nodeVMDetect.conclusion(options);

export const getPercentage = (
  options?: VMInfoOptions<PercentageSettings>,
): Promise<number> => nodeVMDetect.percentage(options);

export const getDetectedTechniques = (
  options?: VMInfoOptions<DetectedTechniquesSettings>,
): Promise<TechniqueFlags[]> => nodeVMDetect.detectedTechniques(options);
