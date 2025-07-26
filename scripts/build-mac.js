import { execSync } from 'child_process';

execSync('rm -rf dist/mac');

execSync('mkdir -p dist/mac/x64');
execSync('arch -x86_64 node-gyp rebuild --arch=x64');
execSync('mv -f build/Release/detectMac.node dist/mac/x64/detectMac.node');
execSync(
  'dsymutil dist/mac/x64/detectMac.node -o dist/mac/x64/detectMac.node.dSYM',
);

execSync('mkdir -p dist/mac/arm64');
execSync('node-gyp rebuild --arch=arm64');
execSync('mv -f build/Release/detectMac.node dist/mac/arm64/detectMac.node');
execSync(
  'dsymutil dist/mac/arm64/detectMac.node -o dist/mac/arm64/detectMac.node.dSYM',
);
