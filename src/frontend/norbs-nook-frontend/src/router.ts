import { createMemoryHistory, createRouter } from 'vue-router'

import WelcomeView from "./views/Welcome.vue"
import SigninRegister from './views/SigninRegister.vue'
import Dashboard from './views/Dashboard.vue'
import Books from './views/Books.vue'
import History from './views/History.vue'
import AccountManagement from './views/AccountManagement.vue'
import ServerDown from './views/ServerDown.vue'

const routes = [
    {path: '/', component: WelcomeView},
    {path: '/login-register', component: SigninRegister},
    {path: '/dashboard', component: Dashboard},
    {path: '/books', component: Books},
    {path: '/history', component: History},
    {path: '/account-management', component: AccountManagement},
    {path: '/server-down', component: ServerDown}
]

export default routes