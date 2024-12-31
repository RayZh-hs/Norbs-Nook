<template>
    <Suspense>
        <template #default>
            <div v-if="account_info.status" style="transform: translateY(-1rem);">
                <div class="middle-wrapper">
                    <h1 class="welcome-header a-fade-in">Welcome back,
                        <span class="welcome-header__username-compound">
                            {{ account_info.content.username }}
                            <div style="display: inline-flex; gap:1rem;">
                                <n-button v-if="account_info.content.privilege == 1" text
                                    style="font-size: 28px; outline: none; transform: translateY(0.2rem);">
                                    <n-tooltip>
                                        <template #trigger>
                                            <n-icon>
                                                <Edit />
                                            </n-icon>
                                        </template>
                                        Account Settings
                                    </n-tooltip>
                                </n-button>
                                <n-button v-if="account_info.content.privilege >= 3" text
                                    style="font-size: 28px; outline: none; transform: translateY(0.2rem);">
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
                                <n-button v-if="account_info.content.privilege >= 3" text
                                    style="font-size: 28px; outline: none; transform: translateY(0.2rem);">
                                    <n-tooltip>
                                        <template #trigger>
                                            <n-icon>
                                                <NotebookReference />
                                            </n-icon>
                                        </template>
                                        Book Management
                                    </n-tooltip>
                                </n-button>
                                <n-button v-if="account_info.content.privilege >= 7" text
                                    style="font-size: 28px; outline: none; transform: translateY(0.2rem);">
                                    <n-tooltip>
                                        <template #trigger>
                                            <n-icon>
                                                <Catalog />
                                            </n-icon>
                                        </template>
                                        Logs
                                    </n-tooltip>
                                </n-button>
                            </div>
                        </span>
                    </h1>
                    <p class="a-fade-in a-delay-2" style="font-size: 1.4rem;letter-spacing: 3pt;">What would you like to
                        read today?</p>
                    <div class="search-bar a-fade-in a-delay-3">
                        <n-input-group>
                            <n-select size="large" v-model:value="query_mode" :options="available_query_modes"
                                :style="{ width: '33%' }" />
                            <n-input size="large" v-if="query_mode == 'all'" placeholder="Show me all" disabled />
                            <n-input size="large" v-else-if="query_mode != 'keywords'"
                                placeholder="Search for books..." />
                            <n-dynamic-tags size="large" v-else placeholder="Search for books..."
                                class="keywords_input" />
                        </n-input-group>
                        <n-button text style="font-size: 24px; outline: none;">
                            <n-icon>
                                <SearchOutline />
                            </n-icon>
                        </n-button>
                    </div>
                </div>
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

.search-bar {
    display: flex;
    justify-content: center;
    margin-top: 3rem;
    gap: 1rem;
}

.keywords_input {
    width: 66%;
    padding-left: 1rem;
    align-items: center;
}
</style>

<script lang="ts" setup>
import { ref, onMounted, watchEffect } from 'vue';
import { useRoute, useRouter } from 'vue-router'; // Import useRoute and useRouter
import axios from 'axios';
import { useMessage } from 'naive-ui';
import { reactive } from 'vue';
import { SearchOutline } from '@vicons/ionicons5';
import { Edit, User, NotebookReference, Catalog } from '@vicons/carbon';

const message = useMessage();
const account_info = ref({ username: 'Guest' });
let cancelTokenSource: any = null;

const route = useRoute();
const router = useRouter();

const available_query_modes = [
    { label: 'All', value: 'all' },
    { label: 'ISBN', value: 'isbn' },
    { label: 'Title', value: 'title' },
    { label: 'Author', value: 'author' },
    { label: 'Keywords', value: 'keywords' }
]
const query_mode = ref("all");

const fetchAccountInfo = async () => {
    try {
        cancelTokenSource && cancelTokenSource.cancel('Request cancelled');
        cancelTokenSource = axios.CancelToken.source();
        const response = await axios.post('http://localhost:5000/api/active_account_info', {
            cancelToken: cancelTokenSource.token
        });
        account_info.value = response.data;
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

</script>