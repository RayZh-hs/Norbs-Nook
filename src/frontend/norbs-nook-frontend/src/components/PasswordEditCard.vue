<script lang="ts" setup>
import axios from 'axios';
import { useMessage } from 'naive-ui';
// @ts-ignore
import { ref, reactive, defineProps, defineEmits } from 'vue';

const message = useMessage();

const props = defineProps<{
    account: Object;
    privilege: number;
}>();

const emit = defineEmits<{
    close: () => void;
    success: () => void;
}>();

const passwordForm = ref({
    oldPassword: '',
    newPassword: '',
    confirmPassword: ''
})

const handleSubmit = async () => {
    if (passwordForm.value.newPassword.length === 0 || passwordForm.value.confirmPassword.length === 0) {
        message.error('New password and confirm password are required');
        return;
    }
    if (passwordForm.value.newPassword != passwordForm.value.confirmPassword) {
        message.error('New password and confirm password do not match');
        return;
    }
    if (props.privilege !== 7 && passwordForm.value.oldPassword.length === 0) {
        message.error('Old password is required');
        return;
    }
    // Pass on to the api to update the password
    const response = await axios.post("http://localhost:5000/api/password", {
        userid: props.account.userid,
        old_password: passwordForm.value.oldPassword,
        new_password: passwordForm.value.newPassword
    });
    if (response.data.status === "success") {
        message.success('Password updated successfully');
        emit('success');
    }
    else {
        message.error(response.data.message);
    }
}

</script>

<template>
    <n-card class="card-color-override password-edit-card" :title="'Edit Password of ' + props.account.username">
        <n-flex vertical gap="small">
            <n-form-item label="User-id">
                <n-input disabled
                :value="props.account.userid"
                />
            </n-form-item>
            <n-form-item label="Old Password">
                <n-input :disabled="props.privilege === 7"
                :placeholder="props.privilege === 7 ? 'Root user can omit old password' : 'Enter your old password'"
                v-model:value="passwordForm.oldPassword"
                />
            </n-form-item>
            <n-form-item label="New Password">
                <n-input type="password" show-count
                v-model:value="passwordForm.newPassword"
                />
            </n-form-item>
            <n-form-item label="Confirm Password">
                <n-input type="password" show-count
                v-model:value="passwordForm.confirmPassword"
                />
            </n-form-item>
            <n-flex align="center" justify="center">
                <n-button @click="emit('close')">Cancel</n-button>
                <n-button @click="handleSubmit">Submit</n-button>
            </n-flex>
        </n-flex>
    </n-card>
</template>

<style scoped>
.card-color-override {
    background-color: #262626;
}

.password-edit-card {
    width: 24rem;
    min-height: 26rem;
    text-align: center;
}
</style>