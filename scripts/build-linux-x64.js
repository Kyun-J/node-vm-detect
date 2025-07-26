import { execSync } from 'child_process';
import fs from 'fs';

fs.mkdirSync('./dist/linux/x64', { recursive: true });
execSync('node-gyp rebuild --arch=x64');
fs.copyFileSync(
  './build/Release/detectLinux.node',
  './dist/linux/x64/detectLinux.node',
);
