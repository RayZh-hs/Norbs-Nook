<script lang="ts" setup>
import axios from 'axios';
import { useMessage } from 'naive-ui';
// @ts-ignore
import { ref, defineEmits } from 'vue';

const message = useMessage();

const emit = defineEmits<{
    close: () => void;
    success: () => void;
}>();

const userForm = ref({
    userid: '',
    username: '',
    password: '',
    privilege: 1
})

const handleSubmit = async () => {
    if (userForm.value.userid.length === 0 || userForm.value.username.length === 0 || userForm.value.password.length === 0) {
        message.error('User-id, Username and Password are required');
        return;
    }
    // Pass on to the api to update the password
    const response = await axios.post("http://localhost:5000/api/add_user", {
        userid: userForm.value.userid,
        username: userForm.value.username,
        password: userForm.value.password,
        privilege: userForm.value.privilege
    });
    if (response.data.status === "success") {
        message.success('User added successfully');
        emit('success');
    }
    else {
        message.error(response.data.message);
        // Do not close the dialog
    }
}

</script>

<template>
    <n-card class="card-color-override password-edit-card" title="New User">
        <n-flex vertical gap="small">
            <n-form-item label="User-id">
                <n-input v-model:value="userForm.userid" />
            </n-form-item>
            <n-form-item label="Username">
                <n-input v-model:value="userForm.username" />
            </n-form-item>
            <n-form-item label="Privilege">
                <n-radio-group v-model:value="userForm.privilege" style="width: 100%;">
                    <n-radio-button style="width: 33.3%;" :value="1">Customer</n-radio-button>
                    <n-radio-button style="width: 33.3%;" :value="3">Employee</n-radio-button>
                    <n-radio-button style="width: 33.3%;" :value="7">Admin</n-radio-button>
                </n-radio-group>
            </n-form-item>
            <n-form-item label="Password">
                <n-input v-model:value="userForm.password" type="password" show-count />
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
    width: 22rem;
    min-height: 26rem;
    text-align: center;
}
</style>