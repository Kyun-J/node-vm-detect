import { defineConfig } from 'vite';
import dts from 'vite-plugin-dts';
import { resolve } from 'path';
import { build } from 'vite';

function workerPlugin() {
  return {
    name: 'worker-plugin',
    async closeBundle() {
      await build({
        configFile: false,
        build: {
          lib: {
            entry: resolve(__dirname, 'src/worker.ts'),
            formats: ['cjs'],
            fileName: () => 'node-vm-detect-child.js',
          },
          rollupOptions: {
            external: ['node:module', 'node:path'],
            output: {
              format: 'cjs',
              exports: 'named',
              globals: {
                'node:module': 'module',
                'node:path': 'path',
              },
            },
          },
          target: 'node18',
          outDir: 'dist',
          emptyOutDir: false,
          minify: true,
        },
      });
    },
  };
}

function cliPlugin() {
  return {
    name: 'cli-plugin',
    async closeBundle() {
      await build({
        configFile: false,
        build: {
          lib: {
            entry: resolve(__dirname, 'src/bin/cli.ts'),
            formats: ['cjs'],
            fileName: () => 'cli.js',
          },
          rollupOptions: {
            external: [
              'node:fs',
              'node:path',
              'node:events',
              'node:process',
              'node:child_process',
            ],
            output: {
              format: 'cjs',
              exports: 'named',
              globals: {
                'node:fs': 'fs',
                'node:path': 'path',
                'node:events': 'events',
                'node:process': 'process',
                'node:child_process': 'child_process',
              },
            },
          },
          target: 'node18',
          outDir: 'dist',
          emptyOutDir: false,
          minify: true,
        },
      });
    },
  };
}

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
      external: ['node:module', 'node:child_process', 'node:path', 'node:url'],
      output: {
        dynamicImportInCjs: false,
        globals: {
          'node:module': 'module',
          'node:child_process': 'child_process',
          'node:path': 'path',
          'node:url': 'url',
        },
        exports: 'named',
      },
    },
    target: 'node18',
    outDir: 'dist',
    minify: true,
  },
  plugins: [
    dts({
      rollupTypes: true,
    }),
    workerPlugin(),
    cliPlugin(),
  ],
});
