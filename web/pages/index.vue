<template>
  <div class="flex flex-col min-h-screen items-center justify-center">
    <div class="w-9/12">
      <UForm :schema="schema" :state="state" class="grid grid-cols-3 gap-5">
        <UFormGroup label="HEX - 16進制" name="hex" eager-validation>
          <UInput
            v-model="state.hex"
            placeholder="16進制"
            @input="userUpdateInputEvent.bind(null, 16)"
          />
        </UFormGroup>
        <UFormGroup label="DEC - 10進制" name="dec" eager-validation>
          <UInput
            v-model="state.dec"
            placeholder="10進制"
            @input="userUpdateInputEvent.bind(null, 10)"
          />
        </UFormGroup>
        <UFormGroup label="BIN - 2進制" name="bin" eager-validation>
          <UInput
            v-model="state.bin"
            placeholder="2進制"
            @input="userUpdateInputEvent.bind(null, 2)"
          />
        </UFormGroup>
      </UForm>
      <div class="grid grid-cols-8 gap-5 m-2 select-none mt-20">
        <div
          v-for="i in 8"
          :key="i"
          class="bg-white text-slate-600 flex items-center justify-center cursor-pointer h-20 rounded-2xl flex-col hover:bg-gray-300 transition-colors duration-200 ease-in-out"
          :class="{
            '!bg-slate-600 !text-white hover:!bg-slate-700': !(
              binValue &
              (1 << (8 - i))
            ),
          }"
          @mouseenter.left="() => pressed && mouseEvent(i, false)"
          @mousedown.left="() => mouseEvent(i, true)"
        >
          <div
            v-text="binValue & (1 << (8 - i)) ? '1' : '0'"
            class="flex items-center justify-center h-full"
          ></div>
          <div class="w-full bg-black h-1"></div>
          <div v-text="i" class="flex items-center justify-center h-full"></div>
        </div>
      </div>
      <div class="grid grid-cols-5 gap-10 mt-20 select-none">
        <UButton block @click="binValue >>= 1">左移一位</UButton>
        <UButton block @click="binValue = 0xff">設定全白</UButton>
        <UButton block @click="binValue = 0x00">設定全黑</UButton>
        <UButton block @click="binValue = ~binValue & 0xff">反轉數值</UButton>
        <UButton block @click="binValue = (binValue << 1) & 0xff">
          右移一位
        </UButton>
      </div>
    </div>
  </div>
</template>

<script lang="ts" setup>
import { reactive, ref } from 'vue';
import { useMousePressed } from '@vueuse/core';
import { useHead } from '@unhead/vue';
import { onKeyStroke, onKeyUp } from '@vueuse/core';
import { z } from 'zod';
import { watch } from 'vue';

useHead({ title: 'UnoCar - 感測器計算' });

const binValue = ref(0);
const moveValue = ref(false);
const shiftPressed = ref(false);
const isUserUpdate = ref(false);
const { pressed } = useMousePressed();

const schema = z.object({
  hex: z.string().regex(/^[a-fA-F0-9]{0,2}$/, '16進制格式錯誤'),
  dec: z.string().regex(/^[0-9]{0,10}$/, '10進制格式錯誤'),
  bin: z.string().regex(/^[01]{0,8}$/, '2進制格式錯誤'),
});
const state = reactive({ hex: '0', dec: '0', bin: '0' });

watch(binValue, () => {
  if (!isUserUpdate.value) {
    userUpdate(binValue.value);
  }
});

onKeyStroke('Shift', (e) => {
  e.preventDefault();
  shiftPressed.value = true;
});

onKeyUp('Shift', () => {
  shiftPressed.value = false;
});

const mouseEvent = (index: number, start: boolean) => {
  const mask = 1 << (8 - index);
  if (start) {
    binValue.value ^= mask;
    moveValue.value = !!(binValue.value & mask);
  } else if (shiftPressed.value) {
    binValue.value ^= mask;
  } else if (moveValue.value) {
    binValue.value |= mask;
  } else {
    binValue.value &= ~mask;
  }
};

const userUpdate = (value: number) => {
  console.log(value);

  state.dec = value.toString();
  state.bin = value.toString(2).padStart(8, '0');
  state.hex = value.toString(16).padStart(2, '0');
};

let timer: NodeJS.Timeout | null = null;
const userUpdateInputEvent = (base: number, event: InputEvent) => {
  const value = event.target.value;

  if (timer) clearTimeout(timer);

  isUserUpdate.value = true;
  binValue.value = parseInt(value, base);

  timer = setTimeout(() => {
    isUserUpdate.value = false;
    userUpdate(binValue.value);
    timer = null;
  }, 800);
};

type InputEvent = { target: { value: string } };
</script>
