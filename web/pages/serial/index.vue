<template>
  <div class="flex flex-col min-h-screen items-center justify-center">
    <div class="w-9/12">
      <div class="grid grid-cols-8 gap-5 m-2 select-none mt-20">
        <div
          v-for="(value, i) in data"
          :key="i"
          class="bg-white text-slate-600 flex items-center justify-center cursor-pointer h-20 rounded-2xl flex-col hover:bg-gray-300 transition-colors duration-200 ease-in-out"
          :class="{
            '!bg-slate-600 !text-white hover:!bg-slate-700': !(
              binValue &
              (1 << (8 - i))
            ),
          }"
        >
          <div
            v-text="value"
            class="flex items-center justify-center h-full"
          ></div>
          <div
            v-text="8 - i"
            class="flex items-center justify-center h-full"
          ></div>
        </div>
      </div>
    </div>
    <UButton class="mt-8" @click="connectSerial()" v-if="!gPort">連線</UButton>
    <UButton class="mt-8" @click="disconnectSerial()" v-else>斷線</UButton>

    <div class="w-9/12">
      <div v-for="(v, i) in dataMap" :key="i">{{ i }}: {{ v }}</div>
    </div>
  </div>
</template>

<script lang="ts" setup>
import { onUnmounted, ref } from 'vue';

const toast = useToast();

const gPort = ref<SerialPort>();
const binValue = ref(0);
const data = ref<number[]>(Array.from({ length: 8 }, () => 0));
const dataMap = ref({} as Record<string, string>);

onUnmounted(async () => {
  disconnectSerial();
});

async function connectSerial() {
  try {
    await gPort.value?.forget();

    const port = await navigator.serial.requestPort();
    await port.open({ baudRate: 9600 });
    gPort.value = port;

    port.addEventListener('disconnect', () => {
      disconnectSerial();
    });

    const decoder = new TextDecoderStream();

    port.readable.pipeTo(decoder.writable);

    const inputStream = decoder.readable;
    const reader = inputStream.getReader();

    let buf: string = '';
    while (true) {
      const { value, done } = await reader.read();
      if (value) {
        buf += value;

        const lines = buf.split('\n');
        if (lines.length > 1) {
          buf = lines.pop() ?? '';

          for (let line of lines) {
            line = line.trim();
            console.log('[readLoop]', line);

            if (line.startsWith('set:')) {
              // set:<name>:<value>
              const [_, name, value] = line.split(':');
              console.log('set:', name, value);
              dataMap.value[name] = value;
            } else if (line.startsWith('alert-')) {
              // alert-message
              toast.add({
                title: line.split('-').slice(1).join('-'),
              });
            } else {
              const match =
                /^((?:[\d]{4} ){8})sensorBools: \d{8} sensorBools: ([0-9a-fA-F]+)$/.exec(
                  line
                );

              if (match) {
                const [_, adcValues, _0, sensorBoolsHex] = match;
                binValue.value = parseInt(sensorBoolsHex, 16);
                data.value = [];

                for (const value of adcValues.trim().split(' ').map(Number)) {
                  data.value.push(value);
                }
              } else {
                console.log('Unrecognized line format:', line);
              }
            }
          }
        }
      }
      if (done) {
        console.log('[readLoop] DONE', done);
        reader.releaseLock();
        break;
      }
    }
  } catch (error) {
    console.error(error);
  }
  disconnectSerial();
}

async function disconnectSerial() {
  await gPort.value?.forget().catch(console.error);
  console.log('port forget');
  dataMap.value = {};
  gPort.value = void 0;
}
</script>

<style lang="scss" scoped></style>
