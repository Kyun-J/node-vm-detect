import { execSync } from 'child_process';
import fs from 'fs';

fs.mkdirSync('./dist/win/arm64', { recursive: true });
execSync('node-gyp rebuild --arch=arm64');
fs.copyFileSync(
  './build/Release/detectWindows.node',
  './dist/win/arm64/detectWindows.node',
);
fs.copyFileSync(
  './build/Release/detectWindows.pdb',
  './dist/win/arm64/detectWindows.pdb',
);
