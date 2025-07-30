import { execSync } from 'child_process';
import fs from 'fs';

fs.mkdirSync('./dist/win/x64', { recursive: true });
execSync('node-gyp rebuild --arch=x64');
fs.copyFileSync(
  './build/Release/vmDetect.node',
  './dist/win/x64/vmDetect.node',
);
