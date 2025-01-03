<script setup lang="ts">
import { RouterView } from 'vue-router';
import { darkTheme, NConfigProvider } from 'naive-ui';

import { ChevronLeft, ChevronRight } from '@vicons/carbon';

</script>

<template>
  <n-config-provider :theme="darkTheme">
    <n-message-provider>
      <n-modal-provider>
        <main class="container">
          <!-- <div class="left-middle-wrapper a-fade-in" style="margin: 2rem; transform: translateY(-2rem);" v-if="$route.path !== '/'">
          <n-button style="font-size: 2rem; outline: none;" text @click="$router.go(-1)">
            <n-icon>
              <ChevronLeft />
            </n-icon>
          </n-button>
        </div> -->
          <RouterView />
          <p class="footer">Norb's Nook @ 2024. Made with ❤️ by Norb</p>
        </main>
      </n-modal-provider>
    </n-message-provider>
  </n-config-provider>
</template>

<style>
.popup {
  position: absolute;
  top: 0;
  left: 0;
  width: 60vw;
  height: 60vh;
  background-color: rgba(0, 0, 0, 0.5);
  display: none;
  justify-content: center;
  align-items: center;
  z-index: 100;
}

.footer {
  position: fixed;
  bottom: 1.4rem;
  left: 50%;
  transform: translateX(-50%);
  text-align: center;

  color: white;
  font-size: 0.8rem;
  font-weight: 200;
  letter-spacing: 0.1rem;

  opacity: 0.5;
  cursor: default;
  user-select: none;
}
</style>
