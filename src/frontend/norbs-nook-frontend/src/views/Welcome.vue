<script setup lang="ts">
import { LogoGithub as GithubIcon } from '@vicons/ionicons5'
import { NIcon } from 'naive-ui'
import { defineComponent, h, ref, onMounted } from 'vue'
import { useRouter } from 'vue-router'

const router = useRouter() // This line is crucial

const renderIcon = () => {
  return h(NIcon, null, {
    default: () => h(GithubIcon)
  })
}

const gotoLogin = () => {
  router.push('/signin') // Use the router instance
}

const gotoRepo = () => {
  window.open('https://github.com/RayZh-hs/Norbs-Nook')
}
</script>

<template>
    <div class="welcome">
        <h1 class="welcome__title a-fade-in">Norb's Nook</h1>
        <p class="welcome__slogan a-fade-in a-delay-5">A place where you can read and wonder</p>
        <n-flex justify="space-around">
            <n-button ghost size="large" class="custom-btn custom-btn--long-btn a-fade-in a-delay-10"
                @click="gotoLogin"
            >Login</n-button>
            <n-button ghost size="large" class="custom-btn custom-btn--long-btn a-fade-in a-delay-10"
                :render-icon="renderIcon" @click="gotoRepo">View
                Source</n-button>
        </n-flex>
        <n-flex justify="center" class="a-fade-in a-delay-12">
            <n-button text size="large" class="sign-up-btn">New here? Click to sign up!</n-button>
        </n-flex>
    </div>
</template>

<style scoped>
.welcome__title {
    font-size: 6rem;
    font-weight: 300;
    margin: 2pt;
}

.welcome__slogan {
    font-size: 2rem;
    font-weight: 300;
}

.custom-btn {
    height: 3rem;
    width: 3rem;
    font-size: 1.2rem;
}

.custom-btn--long-btn {
    width: 45%;
}

.sign-up-btn {
    font-size: 0.9rem;
    font-weight: 400;
    letter-spacing: 1pt;
    margin-top: 3rem;
    opacity: 0.7;
}

</style>