<template>
    <Suspense>
        <template #default>
            <div v-if="account_info.status" style="transform: translateY(-1rem);">
                <div class="middle-wrapper">
                    <h1 class="welcome-header a-fade-in">Welcome back,
                        <span class="welcome-header__username-compound">
                            {{ account_info.content.username }} \
                            <div style="display: inline-flex; gap:1rem;">
                                <n-button v-if="account_info.content.privilege == 1" text style="outline: none;"
                                    :style="{ 'font-size': action_font_size }">
                                    <n-tooltip>
                                        <template #trigger>
                                            <n-icon>
                                                <Edit />
                                            </n-icon>
                                        </template>
                                        Account Settings
                                    </n-tooltip>
                                </n-button>
                                <n-button v-if="account_info.content.privilege >= 3" text style="outline: none;"
                                    :style="{ 'font-size': action_font_size }">
                                    <n-tooltip>
                                        <template #trigger>
                                            <n-icon>
                                                <User />
                                            </n-icon>
                                        </template>
                                        User Management
                                    </n-tooltip>
                                    <!-- <n-icon>
                                        <User />
                                    </n-icon> -->
                                </n-button>
                                <n-button v-if="account_info.content.privilege >= 3" text style="outline: none;"
                                    :style="{ 'font-size': action_font_size }" @click="handleNewBook">
                                    <n-tooltip>
                                        <template #trigger>
                                            <n-icon>
                                                <NotebookReference />
                                            </n-icon>
                                        </template>
                                        New Book
                                    </n-tooltip>
                                </n-button>
                                <n-button v-if="account_info.content.privilege >= 7" text style="outline: none;"
                                    :style="{ 'font-size': action_font_size }" @click="router.push('history')">
                                    <n-tooltip>
                                        <template #trigger>
                                            <n-icon>
                                                <ChartHistogram />
                                            </n-icon>
                                        </template>
                                        History
                                    </n-tooltip>
                                </n-button>
                                <n-button v-if="account_info.content.privilege >= 1" text style="outline: none;"
                                    :style="{ 'font-size': action_font_size }" @click="handleLogout">
                                    <n-tooltip>
                                        <template #trigger>
                                            <n-icon>
                                                <Power />
                                            </n-icon>
                                        </template>
                                        Logout
                                    </n-tooltip>
                                </n-button>
                            </div>
                        </span>
                    </h1>
                    <p class="a-fade-in a-delay-2" style="font-size: 1.4rem;letter-spacing: 3pt;">What would you like to
                        {{account_info.content.privilege <= 1 ? 'read' : 'do'}} today?
                    </p>
                    <search-bar class="a-fade-in a-delay-3" @search="handleSearch" />
                </div>
                <n-modal v-model:show="showAddBook">
                    <BookEditCard onIsbnCollide="abort" cardTitle="New Book"
                        @close="showAddBook = false"
                        @modify="showAddBook = false; message.success('Book added!')"
                    />
                </n-modal>
            </div>
            <div v-else>
                <p>Loading account information...</p>
            </div>
        </template>
        <template #fallback>
            <h1>Loading...</h1>
        </template>
    </Suspense>
</template>

<style scoped>
.welcome-header {
    font-size: 2.5rem;
    margin-bottom: 2rem;
}
</style>

<script lang="ts" setup>
import { ref, onMounted, watchEffect } from 'vue';
import { useRoute, useRouter } from 'vue-router'; // Import useRoute and useRouter
import axios from 'axios';
import { useMessage } from 'naive-ui';
import { reactive } from 'vue';
import { SearchOutline } from '@vicons/ionicons5';
import { Edit, User, NotebookReference, Catalog, Power, ChartHistogram } from '@vicons/carbon';

import SearchBar from '../components/SearchBar.vue';
import BookEditCard from '../components/BookEditCard.vue';

const message = useMessage();
const account_info = ref({ username: 'Guest' });
let cancelTokenSource: any = null;

const action_font_size = '24px';

const route = useRoute();
const router = useRouter();

const showAddBook = ref(false);
const handleNewBook = () => {
    showAddBook.value = true;
};

const fetchAccountInfo = async () => {
    try {
        cancelTokenSource && cancelTokenSource.cancel('Request cancelled');
        cancelTokenSource = axios.CancelToken.source();
        const response = await axios.post('http://localhost:5000/api/active_account_info', {
            cancelToken: cancelTokenSource.token
        });
        account_info.value = response.data;
        // Save the account info to local storage:
        localStorage.setItem('account_info', JSON.stringify(response.data));
    } catch (error) {
        if (axios.isCancel(error)) {
            console.log('Request cancelled:', error.message);
        } else {
            console.error("Error fetching account info:", error);
            message.error('Failed to fetch account info');
        }
    }
};

// Use watchEffect to react to route changes
watchEffect(() => {
    fetchAccountInfo();
});

const handleSearch = (search_data: any) => {
    console.log('Search data:', search_data);
    // Save the data to local storage:
    localStorage.setItem('search_data', JSON.stringify(search_data));
    // Then redirect to the Books page:
    router.push("books");
};

const handleLogout = async () => {
    try {
        const response = await axios.post('http://localhost:5000/api/logout');
        if (response.data.status == 'success') {
            message.success('Logged out successfully');
            // Clear the account info from local storage:
            localStorage.removeItem('account_info');
            // Redirect to the login page if the user_stack is empty:
            if (response.data.more_users == false) {
                // router.push("login-register");
                router.back();
            }
            // Update the current info:
            fetchAccountInfo().then(() => {
                message.info('Current User: ' + (account_info.value.content.username ? account_info.value.content.username : 'Guest'));
            });
        } else {
            message.error('Failed to logout');
        }
    } catch (error) {
        console.error("Error logging out:", error);
        message.error('Failed to logout');
    }
}

</script>