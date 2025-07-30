import { execSync } from 'child_process';
import fs from 'fs';

fs.mkdirSync('./dist/linux/arm64', { recursive: true });
execSync('node-gyp rebuild --arch=arm64');
fs.copyFileSync(
  './build/Release/vmDetect.node',
  './dist/linux/arm64/vmDetect.node',
);
