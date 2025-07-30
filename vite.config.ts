import { defineConfig } from 'vite';
import dts from 'vite-plugin-dts';
import { resolve } from 'path';

export default defineConfig({
  build: {
    lib: {
      entry: resolve(__dirname, 'src/index.ts'),
      formats: ['es', 'cjs'],
      fileName: (format) => {
        if (format === 'es') return 'index.esm.js';
        if (format === 'cjs') return 'index.cjs';
        return `index.${format}.js`;
      },
    },
    rollupOptions: {
      external: ['node:module'],
      output: {
        dynamicImportInCjs: false,
        globals: {
          'node:module': 'module',
        },
        exports: 'named',
      },
    },
    target: 'node18',
    outDir: 'dist',
    sourcemap: true,
    minify: true,
  },
  plugins: [
    dts({
      rollupTypes: true,
    }),
  ],
});
