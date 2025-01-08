<script lang="ts" setup>
import { useRouter } from 'vue-router';

const router = useRouter();

</script>

<template>
    <div class="server-down-container">
        <h1 class="a-fade-in">The server has shutdown</h1>
        <p class="a-fade-in a-delay-3">To restart the backend server, navigate to 
            <code>src/api</code>
            and run:
        </p>
        <div class="instruction-code-wrapper a-fade-in a-delay-6">
            <code class="block-code"><span class="reserved">source</span> venv/bin/activate</code>
            <code class="block-code">python server.py <span class="comments"># Change to your deployment server</span></code>
        </div>
        <p class="a-fade-in a-delay-8">After doing so, you can <a @click="router.replace('/')">click here</a> to return to the main page.</p>
    </div>
</template>

<style scoped>
.server-down-container {
    transform: translateY(-1rem);
}

h1 {
    font-weight: 200;
}

p {
    font-weight: 300;
    letter-spacing: 0.5pt;
}

a {
    color: #6cdab5;
    cursor: pointer;
    transition: color 0.2s;
}
a:hover {
    color: #7fe7c4;
}

.instruction-code-wrapper {
    display: flex;
    flex-direction: column;
    align-items: flex-start;
    background-color: #1c1b1b;
    gap: 0.5rem;
    padding: 1rem;
    border-radius: 5px;
}

.block-code {
    font-weight: 200;
    font-size: small;
    font-family: Consolas, monospace;
}

.block-code::before {
    color: orange;
    content: "$ ";
}

.reserved {
    color: #569cd6;
    font-weight: bold;
}

.comments {
    color: #696666;
}
</style>