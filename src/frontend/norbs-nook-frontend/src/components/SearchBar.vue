<script>
import { SearchOutline } from '@vicons/ionicons5';
import { ref, defineComponent } from 'vue';

export default defineComponent({
    props: {
        query_mode: { type: String, default: 'all' },
        query_input: { type: String, default: '' },
        query_tags: { type: Array, default: () => [] },
    },
    setup(props, { emit }) {
        const available_query_modes = [
            { label: 'All', value: 'all' },
            { label: 'ISBN', value: 'isbn' },
            { label: 'Title', value: 'title' },
            { label: 'Author', value: 'author' },
            { label: 'Keywords', value: 'keywords' }
        ];

        const internalQueryMode = ref(props.query_mode);
        const internalQueryInput = ref(props.query_input);
        const internalQueryTags = ref(props.query_tags);

        const performSearch = () => {
            emit('search', {
                query_mode: internalQueryMode.value || props.query_mode,
                // query_content: (internalQueryMode.value === 'keywords') ? internalQueryTags.value : internalQueryInput.value,
                query_content: internalQueryInput.value,
                query_tags: internalQueryTags.value,
            });
        };

        return {
            available_query_modes,
            internalQueryMode,
            internalQueryInput,
            internalQueryTags,
            performSearch,
        };
    },
    components: {
        SearchOutline,
    },
    emits: ['search'],
});
</script>

<template>
    <div class="search-bar">
        <n-input-group>
            <n-select size="large" v-model:value="internalQueryMode" :options="available_query_modes" :style="{ width: '33%' }" />
            <n-input size="large" v-if="internalQueryMode == 'all'" placeholder="Show me all" disabled />
            <n-input v-model:value="internalQueryInput" size="large" v-else-if="internalQueryMode != 'keywords'" placeholder="Search for books..." />
            <n-dynamic-tags v-model:value="internalQueryTags" size="large" v-else placeholder="Search for books..." class="keywords_input" />
        </n-input-group>
        <n-button text style="font-size: 24px; outline: none;" @click="performSearch">
            <n-icon>
                <SearchOutline />
            </n-icon>
        </n-button>
    </div>
</template>

<style scoped>
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