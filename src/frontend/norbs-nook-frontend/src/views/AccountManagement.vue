<script lang="ts" setup>
import { UserAvatar, GroupAccount, CircleFilled, Edit, AddAlt } from '@vicons/carbon';
import { Power, UserFollow, Close, StatusChange } from '@vicons/carbon';
import axios from 'axios';
// @ts-ignore  < false alarm by vscode >
import { ref, onMounted } from 'vue';
import { useRouter } from 'vue-router';
import PasswordEditCard from '../components/PasswordEditCard.vue';
import AddAccountCard from '../components/AddAccountCard.vue';

const router = useRouter();

const user_count = ref(0);
const login_stack_size = ref(0);
const accounts = ref<any>([]);
const loginStack = ref<any>([]);

const activeUser = ref<number>(-1);
const showEditPassword = ref(false);
const showNewUser = ref(false);

const fetchAccountInfo = async () => {
    const accountsResponse = await axios.post("http://localhost:5000/api/get_all_accounts");
    if (accountsResponse.data.status === "success") {
        accounts.value = accountsResponse.data.content;
        user_count.value = accounts.value.length;
    }
    else {
        console.error(accountsResponse.data.message);
    }
    const loginStackResponse = await axios.post("http://localhost:5000/api/get_login_stack");
    if (loginStackResponse.data.status === "success") {
        loginStack.value = loginStackResponse.data.content;
        login_stack_size.value = loginStack.value.length;
    }
    else {
        console.error(loginStackResponse.data.message);
    }
};

const mapPrivilegeToTimelineType = (privilege: number) => {
    switch (privilege) {
        case 0:
            return "default";
        case 1:
            return "info";
        case 3:
            return "success";
        case 7:
            return "warning";
        default:
            return "default";
    }
};

const mapPrivilegeToName = (privilege: number) => {
    switch (privilege) {
        case 0:
            return "Guest";
        case 1:
            return "Customer";
        case 3:
            return "Employee";
        case 7:
            return "Shopkeeper";
        default:
            return "User";
    }
};

onMounted(async () => {
    console.log("Account Management Mounted");
    await fetchAccountInfo();
    console.log("Accounts: ", accounts.value);
    console.log("Login Stack: ", loginStack.value);
});

</script>

<template>
    <div class="account-management-container">
        <h1 class="account-management__title a-fade-in">Account Management</h1>
        <n-divider />
        <div class="account-management-body-container">
            <Transition>
                <div class="account-management-head-section-container" v-if="user_count > 0">
                    <div class="account-management-head-left-container">
                        <div class="account-data-panel-container">
                            <n-statistic label="Total Accounts" :value="user_count">
                                <template #prefix>
                                    <n-icon>
                                        <UserAvatar />
                                    </n-icon>
                                </template>
                            </n-statistic>
                            <n-statistic label="Login Stack" :value="login_stack_size">
                                <template #prefix>
                                    <n-icon>
                                        <GroupAccount />
                                    </n-icon>
                                </template>
                            </n-statistic>
                        </div>
                        <div class="login-stack-container">
                            <!-- <span>Login Stack</span> -->
                            <!-- Timeline is used as a display container for the Login Stack -->
                            <Transition>
                                <n-timeline horizontal v-if="login_stack_size > 0">
                                    <n-timeline-item v-for="(account, i) in loginStack" :key="i"
                                        :type="mapPrivilegeToTimelineType(account.privilege)" :title="account.username"
                                        :style="{ 'padding-right': i === login_stack_size - 1 ? '0' : '40px' }">
                                        <template #icon v-if="i === login_stack_size - 1">
                                            <n-icon>
                                                <CircleFilled />
                                            </n-icon>
                                        </template>
                                    </n-timeline-item>
                                </n-timeline>
                            </Transition>
                        </div>
                    </div>
                    <n-divider vertical style="height: 5rem;"></n-divider>
                    <!-- The right part of the head is the toolbar -->
                    <div class="account-management-head-right-container">
                        <n-tooltip trigger="hover" placement="right">
                            <template #trigger>
                                <n-button text disabled>
                                    <n-icon size="20">
                                        <Power />
                                    </n-icon>
                                </n-button>
                            </template>
                            Logout All (not developed)
                        </n-tooltip>
                        <n-tooltip trigger="hover" placement="right">
                            <template #trigger>
                                <n-button text @click="showNewUser = true">
                                    <n-icon size="20">
                                        <UserFollow />
                                    </n-icon>
                                </n-button>
                            </template>
                            New User
                        </n-tooltip>
                        <n-tooltip trigger="hover" placement="right">
                            <template #trigger>
                                <n-button text @click="router.push('/login-register')">
                                    <n-icon size="20">
                                        <StatusChange />
                                    </n-icon>
                                </n-button>
                            </template>
                            Change User
                        </n-tooltip>
                    </div>
                </div>
            </Transition>
            <n-scrollbar style="max-height: 28vh; min-width: 120%;" class="a-fade-in">
                <!-- All the users will be listed here -->
                <div v-for="(account, i) in accounts" :key="account.userid" class="account-card">
                    <n-icon size="26">
                        <UserAvatar />
                    </n-icon>
                    <span class="account-card__userid">{{ account.userid }} ({{ account.username }})</span>
                    <div class="tags">
                        <n-tag :type="mapPrivilegeToTimelineType(account.privilege)" size="small"
                            class="account-card__tag">{{
                                mapPrivilegeToName(account.privilege)
                            }}</n-tag>
                    </div>
                    <n-tooltip trigger="hover" placement="right">
                        <template #trigger>
                            <n-button text class="account-card__edit-btn"
                                @click="activeUser = i; showEditPassword = true">
                                <n-icon size="large">
                                    <Edit />
                                </n-icon>
                            </n-button>
                        </template>
                        Edit password
                    </n-tooltip>
                </div>
                <!-- Then there is an ADD button for AddUser -->
                <n-button text class="add-account-btn" @click="showNewUser = true">
                    Click to add new user
                </n-button>
            </n-scrollbar>
        </div>
    </div>
    <n-modal v-model:show="showEditPassword">
        <PasswordEditCard :account="accounts[activeUser]" :privilege="loginStack[login_stack_size - 1].privilege"
            @close="showEditPassword = false" @success="showEditPassword = false" />
    </n-modal>
    <n-modal v-model:show="showNewUser">
        <AddAccountCard :privilege="loginStack[login_stack_size - 1].privilege" @close="showNewUser = false"
            @success="showNewUser = false; fetchAccountInfo();" />
    </n-modal>
</template>

<style scoped>
.account-management__title {
    font-size: 1.8rem;
    font-weight: 20;
}

.account-management-head-section-container {
    display: flex;
    align-items: center;
    justify-content: space-between;
    width: 100%;
}

.account-management-head-left-container {
    width: 100%;
    display: flex;
    flex-direction: column;
    gap: 1rem;
}

.account-management-head-right-container {
    padding: 0.5rem;
    height: 100%;
    /* border: #8a8a8a 0.5px solid; */
    display: flex;
    flex-direction: column;
    gap: 1rem;
}

.account-data-panel-container {
    width: 100%;
    display: flex;
    justify-content: space-evenly;
    align-items: center;
}

.login-stack-container {
    display: flex;
}

.account-management-body-container {
    display: flex;
    flex-direction: column;
    gap: 2rem;
    justify-content: center;
    align-items: center;
}

.account-card {
    width: 100%;
    height: 3rem;
    background-color: #222222;
    border: #f4f4f422 0.5px solid;
    border-radius: 5px;
    margin: 10px 0;
    display: flex;
    justify-content: flex-start;
    gap: 1rem;
    align-items: center;
    padding: 0 20px;
    transition: all 0.3s;
}

.account-card:hover {
    background-color: #30303074;
}

.account-card__userid {
    font-size: 1.2rem;
    font-weight: 20;
}

.account-card__edit-btn {
    margin-left: auto;
}

.add-account-btn {
    color: #8a8a8a;
    font-size: 0.8rem;
    font-weight: 400;
    margin-top: 1.2rem;
    outline: none;
}

/* Animations */
.v-enter-active,
.v-leave-active {
    transition: opacity 0.5s ease;
}

.v-enter-from,
.v-leave-to {
    opacity: 0;
}
</style>