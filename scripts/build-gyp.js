import { execSync } from 'child_process';
import fs from 'fs';

const { platform, arch } = process;

fs.mkdirSync(`./dist/${platform}/${arch}`, { recursive: true });
execSync(`node-gyp rebuild --arch=${arch}`);
fs.copyFileSync(
  './build/Release/vmDetect.node',
  `./dist/${platform}/${arch}/vmDetect.node`,
);
