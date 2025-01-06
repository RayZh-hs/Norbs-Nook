import { createMemoryHistory, createRouter } from 'vue-router'

import WelcomeView from "./views/Welcome.vue"
import SigninRegister from './views/SigninRegister.vue'
import Dashboard from './views/Dashboard.vue'
import Books from './views/Books.vue'
import History from './views/History.vue'

const routes = [
    {path: '/', component: WelcomeView},
    {path: '/login-register', component: SigninRegister},
    {path: '/dashboard', component: Dashboard},
    {path: '/books', component: Books},
    {path: '/history', component: History}
]

export default routes