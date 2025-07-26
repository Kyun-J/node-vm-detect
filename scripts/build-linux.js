import { execSync } from 'child_process';
import fs from 'fs';

fs.mkdirSync('./dist/linux/x64', { recursive: true });
execSync('node-gyp rebuild --arch=x64');
fs.copyFileSync(
  './build/Release/detectLinux.node',
  './dist/linux/x64/detectLinux.node',
);

fs.mkdirSync('./dist/linux/arm64', { recursive: true });
execSync('node-gyp rebuild --arch=arm64');
fs.copyFileSync(
  './build/Release/detectLinux.node',
  './dist/linux/arm64/detectLinux.node',
);
