import { createMemoryHistory, createRouter } from 'vue-router'

import WelcomeView from "./views/Welcome.vue"
import SigninRegister from './views/SigninRegister.vue'
import Dashboard from './views/Dashboard.vue'

const routes = [
    {path: '/', component: WelcomeView},
    {path: '/signin', component: SigninRegister},
    {path: '/dashboard', component: Dashboard}
]

export default routes