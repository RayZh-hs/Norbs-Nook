<script lang="ts">
import { SearchOutline } from '@vicons/ionicons5';
import { ref, defineComponent } from 'vue';
import { type FormInst } from 'naive-ui';
import { useMessage } from 'naive-ui';
import { reactive } from 'vue';
import axios from 'axios';

export default defineComponent({
    props: {
        isbn: { type: String, default: '' },
        original_isbn: {type: String, default: ''},
        title: { type: String, default: '' },
        author: { type: String, default: '' },
        keywords: { type: Array, default: () => [] },
        price: { type: Number, default: 0 },
        cardTitle: { type: String, default: 'Edit Book' },
        onIsbnCollide: { type: String, default: 'abort', validator: (value: string) => ['abort', 'warn', 'ignore'].includes(value) },
    },
    setup(props: any, { emit }) {

        const message = useMessage();
        const editFormRef = ref<FormInst | null>(null);

        const editForm = reactive({
            isbn: props.isbn,
            title: props.title,
            author: props.author,
            keywords: props.keywords,
            price: String(props.price),
        })

        const handleModify = async () => {
            const queryData = {
                isbn: editForm.isbn,
                original_isbn: props.original_isbn ? props.original_isbn : editForm.isbn,
                title: editForm.title,
                author: editForm.author,
                keywords: editForm.keywords,
                price: editForm.price,
                on_collision: props.onIsbnCollide,
            };
            // All api calls here
            const response = await axios.post('http://localhost:5000/api/modify', queryData);
            // Handle failed response
            console.log(response);
            if (response.data?.status === 'failed' || response.data?.status === 'error') {
                message.error(response.data.message);
                // Handle failed response
                console.log(response.data);
                return;
            }
            // Emit the event to the parent component
            emit('modify');
        }

        const handleClose = () => {
            // Close the modal
            emit('close');
        }

        return {
            handleModify,
            handleClose,
            editFormRef,
            editForm,
        }
    },
    components: {
    },
    emits: ['modify', 'close'],
});
</script>

<template>
    <n-card :title="cardTitle" class="card-color-override book-edit-card">
        <n-form :model="editFormRef">
            <n-form-item label="Isbn">
                <n-input v-model:value="editForm.isbn" placeholder="ISBN" />
            </n-form-item>
            <n-form-item label="Title">
                <n-input v-model:value="editForm.title" placeholder="Title" />
            </n-form-item>
            <n-form-item label="Author">
                <n-input v-model:value="editForm.author" placeholder="Author" />
            </n-form-item>
            <n-form-item label="Keywords">
                <n-dynamic-tags v-model:value="editForm.keywords" placeholder="Keywords" />
            </n-form-item>
            <n-form-item label="Price">
                <n-input v-model:value="editForm.price" placeholder="Price">
                    <template #suffix>$</template>
                </n-input>
            </n-form-item>
        </n-form>
        <n-divider />
        <n-flex align="center" justify="center" style="gap: 2.5rem;">
            <n-button type="default" @click="handleClose">Cancel</n-button>
            <n-button type="primary" @click="handleModify">Submit</n-button>
        </n-flex>
    </n-card>
</template>

<style scoped>
.card-color-override {
    background-color: #262626;
}

.book-edit-card {
    width: 24rem;
    min-height: 26rem;
    text-align: center;
}
</style>