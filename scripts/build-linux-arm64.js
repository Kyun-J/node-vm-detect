import { execSync } from 'child_process';
import fs from 'fs';

fs.mkdirSync('./dist/linux/arm64', { recursive: true });
execSync('node-gyp rebuild --arch=arm64');
fs.copyFileSync(
  './build/Release/detectLinux.node',
  './dist/linux/arm64/detectLinux.node',
);
