<script setup lang="ts">
import { RouterView } from 'vue-router';
import { darkTheme, NConfigProvider } from 'naive-ui';
</script>

<template>
  <n-config-provider :theme="darkTheme">
    <n-message-provider>
      <main class="container">
        <RouterView />
        <p class="footer">Norb's Nook @ 2024. Made with ❤️ by Norb</p>
      </main>
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
  position: absolute;
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
