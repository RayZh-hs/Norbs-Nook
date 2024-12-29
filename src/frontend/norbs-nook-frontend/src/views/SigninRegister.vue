<template>
    <div class="view-box">
        <div class="u-fill" :class="{ register_mode: register }">
            <div class="signin__box">
                <h1 class="signin__title a-fade-in">Sign In</h1>
                <n-form :model="signinForm" :rules="signin_rules" ref="signinFormRef">
                    <n-form-item label="User ID" path="userid" size="large" class="a-fade-in a-delay-2">
                        <n-input v-model:value="signinForm.userid" placeholder="Enter your username" />
                    </n-form-item>
                    <n-form-item label="Password" path="password" size="large" class="a-fade-in a-delay-3">
                        <n-input v-model:value="signinForm.password" type="password"
                            placeholder="Enter your password" />
                    </n-form-item>
                    <n-form-item class="a-fade-in a-delay-4" style="margin: 0 0;">
                        <n-checkbox v-model:checked="signinForm.remember">Remember me</n-checkbox>
                    </n-form-item>
                    <n-form-item class="a-fade-in a-delay-5">
                        <n-button type="primary" @click="handleSubmitSignin" class="full-width-button">Sign
                            In</n-button>
                    </n-form-item>
                </n-form>
                <a class="link-away link-away--to-reg a-fade-in a-delay-7" @click="register = true">Not yet registered?
                    Click here to join.</a>
            </div>
            <div class="register__box">
                <h1 class="signin__title a-fade-in">Register</h1>
                <n-form :model="registerForm" :rules="register_rules" ref="registerFormRef">
                    <n-form-item label="User ID" path="userid" size="large">
                        <n-input v-model:value="registerForm.userid" placeholder="Enter your username" />
                    </n-form-item>
                    <n-form-item label="Password" path="password" size="large">
                        <n-input v-model:value="registerForm.password" type="password"
                            placeholder="Enter your password" />
                    </n-form-item>
                    <n-form-item label="Username" path="username" size="large">
                        <n-input v-model:value="registerForm.username" placeholder="Enter your password" />
                    </n-form-item>
                    <n-form-item>
                        <n-button type="primary" @click="handleSubmitRegister" class="full-width-button">Sign
                            Up</n-button>
                    </n-form-item>
                </n-form>
                <a class="link-away link-away--to-signin" @click="register = false">Have an account? Sign in here.</a>
            </div>
        </div>
    </div>
</template>

<script lang="ts" setup>
import { ref, reactive } from 'vue'
import { useMessage, type FormInst } from 'naive-ui'
import { useRouter } from 'vue-router';
import axios from 'axios';

const router = useRouter();
const message = useMessage();
const signinFormRef = ref<FormInst | null>(null);
const registerFormRef = ref<FormInst | null>(null);

const register = ref(false);

const signinForm = reactive({
    userid: '',
    password: '',
    remember: false,
});

const registerForm = reactive({
    userid: '',
    username: '',
    password: '',
});

const signin_rules = {
    userid: {
        required: true,
        message: 'Please enter your user-id',
    },
    password: {
        required: true,
        message: 'Please enter your password',
    },
};

const register_rules = {
    userid: {
        required: true,
        message: 'Please enter your user-id',
    },
    password: {
        required: true,
        message: 'Please enter your password',
    },
    username: {
        required: true,
        message: 'Please enter your username',
    },
}

const handleSubmitSignin = async () => {
    try {
        const isValid = await signinFormRef.value?.validate();
        if (!isValid) return;

        // TODO: Get this real api call working!
        const response = await axios.post('http://localhost:5000/api/login', { user_id: signinForm.userid, password: signinForm.password });
        console.log("Got response: ", response);
        // if (response.success) {
        //     message.success('Sign in successful!');
        //     await router.push('/dashboard'); // Replace with your desired route
        // } else {
        //     message.error(response.message);
        // }
        if (response.data.status == 'success') {
            message.success('Welcome back!');
            await router.push('/dashboard'); // Replace with your desired route
        } else {
            message.error(response.data.message);
        }
    } catch (error) {
        message.error('An error occurred during signing in');
        console.error('Sign In error:', error);
    }
};

//// Simulate API call.
// const simulateSignin = async (userData: any) => {
//     await new Promise(resolve => setTimeout(resolve, 500)); // Simulate network latency
//     if (userData.userid === 'root' && userData.password === 'sjtu') {
//         return { success: true, message: '' };
//     } else {
//         return { success: false, message: 'Invalid username or password' };
//     }
// };

const handleSubmitRegister = async () => {
    try {
        const isValid = await registerFormRef.value?.validate();
        if (!isValid) return;

        const response = await axios.post('http://localhost:5000/api/register', { user_id: registerForm.userid, password: registerForm.password, username: registerForm.username });
        if (response.data.status == 'success') {
            message.success('Welcome to the community!');
            message.success('Please log in once more to continue');
        } else {
            message.error(response.data.message);
        }
    } catch (error) {
        message.error('An error occurred during sign in');
        console.error('Register error:', error);
    }
};

// Simulate API call.
const simulateRegister = async (userData: any) => {
    await new Promise(resolve => setTimeout(resolve, 500)); // Simulate network latency
    if (userData.username === 'root' && userData.password === 'sjtu') {
        return { success: true, message: '' };
    } else {
        return { success: false, message: 'Invalid username or password' };
    }
};

</script>

<style scoped>
.view-box {
    /* background-color: black; */
    position: absolute;
    top: 49%;
    left: 50%;
    transform: translate(-50%, -50%);
    width: 28rem;
    height: 40rem;
    padding: 2rem;
    overflow: hidden;
}

.signin__title {
    font-size: 3rem;
    font-weight: 200;
    margin-bottom: 5rem;
}

.full-width-button {
    /*center the button*/
    margin: 0 auto;
    width: 80%;
    height: 3rem;
}

.link-away {
    font-size: 0.85rem;
    font-weight: 200;

    color: rgb(233, 233, 233);
    text-decoration: none;

    cursor: pointer;
}

.link-away:hover {
    text-decoration: underline;
}

.signin__box,
.register__box {
    position: absolute;
    left: 0;
    top: 0;
    padding: 2rem;
    width: 100%;
    height: 100%;
    /* background-color: rgba(0, 0, 0, 0.5); */

    transition: all 0.3s ease-out;
}

.register__box {
    transform: translateX(100%);
}

.register_mode .register__box {
    transform: translateX(0);
}

.register_mode .signin__box {
    transform: translateX(-100%);
}
</style>