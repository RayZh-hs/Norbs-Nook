<script setup lang="ts">
import { ChartLine, UserData, Data2 } from '@vicons/carbon';
// @ts-ignore  < false alarm by vscode >
import { ref, Transition, onMounted, nextTick } from 'vue';
import { useMessage } from 'naive-ui';
import {
    Chart as ChartJS,
    Title,
    Tooltip,
    Legend,
    BarElement,
    LineElement,
    PointElement,
    CategoryScale,
    LinearScale
} from 'chart.js';
import { Bar } from 'vue-chartjs';
import axios from 'axios';

ChartJS.register(CategoryScale, LinearScale, BarElement, LineElement, PointElement, Title, Tooltip, Legend);

const activeTab = ref('transactions');

const message = useMessage();

const chartData = ref<any>(null);
const chartOptions = ref({
    responsive: true,
    maintainAspectRatio: false,
    scales: {
        y: {
            beginAtZero: false, // Allow negative values on y-axis
            suggestedMin: -50,  // Adjust as needed
            suggestedMax: 100, // Adjust as needed
        },
    },
});
const chartRef = ref(null);
const chartResponded = ref(false);

const chartExtract = ref({
    purchases: 0,
    imports: 0,
    revenue: 0
})

const fetchData = async () => {
    /** This demo can be used as a reference for the data structure:
     * {
    labels: ['January', 'February', 'March', 'April', 'May', 'June', 'July'],
    datasets: [
        {
            type: 'bar',
            label: 'Profit/Loss',
            backgroundColor: 'rgba(127, 231, 196, 0.6)', // Light green with transparency
            borderColor: 'rgba(225, 225, 225, 0.86)', // Light green border
            borderWidth: 1,
            data: [65, -20, 80, -10, 56, 55, -40], // Positive and negative values
        },
        {
            type: 'line',
            label: 'Trend',
            borderColor: '#5acea7', // Dark green line
            borderWidth: 2,
            data: [50, 40, 70, 60, 65, 75, 30],  // Some trend data
            fill: false, // Don't fill under the line
            tension: 0.4 // Add a slight curve to the line
        },
    ],
}
     */
    const response = await axios.post("http://localhost:5000/api/get_transactions");
    chartResponded.value = true;
    console.log(response.data);
    if (response.data.status == 'success') {
        const processedData = {
            labels: response.data.content.map((_: any, i: any) => `Transaction-${i}`),
            datasets: [
                {
                    type: 'bar',
                    label: 'Purchase/Import',
                    backgroundColor: 'rgba(127, 231, 196, 0.6)', // Light green with transparency
                    borderColor: 'rgba(225, 225, 225, 0.86)', // White boarder
                    borderWidth: 1,
                    data: response.data.content.map((item: number) => item.toFixed(2))
                },
                {
                    type: 'line',
                    label: 'Total',
                    borderColor: '#5acea7', // Dark green line
                    borderWidth: 2,
                    // The data is the prefix sum of the profit/loss
                    data: response.data.content.map((item: number, i: number) => {
                        return response.data.content.slice(0, i + 1).reduce((a: number, b: number) => a + b).toFixed(2);
                    }),
                    fill: false, // Don't fill under the line
                    tension: 0.4 // Add a slight curve to the line
                }
            ]
        }
        // Record the purchases, imports and total revenue
        chartExtract.value = {
            purchases: response.data.content.filter((item: number) => item > 0).length,
            imports: response.data.content.filter((item: number) => item < 0).length,
            revenue: response.data.content.reduce((a: number, b: number) => a + b).toFixed(2)
        }
        // Register processedData into the chart
        console.log('processedData:', chartData.value);
        return processedData;
    } else {
        message.error(response.data.message);
    }
}

onMounted(async () => {
    chartData.value = await fetchData();;
    // @ts-ignore  < false alarm by vscode >
    if (chartRef.value) {
        chartRef.value.renderChart(chartData.value, chartOptions.value);
    }
});
</script>

<template>
    <div class="history-container a-fade-in">
        <div class="history-container">
            <div class="tabs">
                <n-tooltip trigger="hover">
                    <template #trigger>
                        <n-button text class="tabs__btn tabs__btn--transactions" @click="activeTab = 'transactions'"
                            :class="{ 'tabs_btn--active': activeTab === 'transactions' }">
                            <n-icon>
                                <ChartLine />
                            </n-icon>
                        </n-button>
                    </template>
                    Transactions
                </n-tooltip>
                <n-tooltip trigger="hover">
                    <template #trigger>
                        <n-button text class="tabs__btn tabs__btn--worker-report" @click="activeTab = 'worker-report'"
                            :class="{ 'tabs_btn--active': activeTab === 'worker-report' }">
                            <n-icon>
                                <UserData />
                            </n-icon>
                        </n-button>
                    </template>
                    Worker Report
                </n-tooltip>
                <n-tooltip trigger="hover">
                    <template #trigger>
                        <n-button text class="tabs__btn tabs__btn--logs" @click="activeTab = 'logs'"
                            :class="{ 'tabs_btn--active': activeTab === 'logs' }">
                            <n-icon>
                                <Data2 />
                            </n-icon>
                        </n-button>
                    </template>
                    Logs
                </n-tooltip>
            </div>
            <n-divider />
            <div class="main-body a-fade-in a-delay-2">
                <Transition>
                    <div class="transactions-body" v-if="activeTab === 'transactions'">
                        <!-- <canvas id="transactionChart"></canvas> -->
                        <!-- <ChartComponent :chart-data="chartData" :chart-options="chartOptions" /> -->
                        <div class="transaction-wrapper" v-if="chartResponded && chartData">
                            <h1 class="history-income-tot">
                                Transactions
                            </h1>
                            <div class="bar-wrapper">
                                <Bar :data="chartData" :options="chartOptions" />
                            </div>
                            <p>{{ chartExtract?.purchases }} purchases, {{ chartExtract?.imports }} imports are recorded in this season.</p>
                            <p>Total revenue generated: <span>${{ chartExtract?.revenue }}</span></p>
                        </div>
                        <!-- <div v-else-if="chartResponded">
                            <p>You are not allowed to view this page.</p>
                            <p>Please login first.</p>
                        </div> -->
                        <div v-else>
                            <n-space vertical>
                                <n-skeleton height="1.3rem" width="33%" />
                                <n-skeleton height="1.3rem" width="66%" :sharp="false" />
                                <n-skeleton height="1.3rem" round />
                                <n-skeleton height="1.3rem" circle />
                            </n-space>
                        </div>
                    </div>
                </Transition>
            </div>
        </div>
    </div>
</template>

<style scoped>
.history-container {
    /* to adjust for the footnote */
    transform: translateY(-1.2rem);
}

/* Tabs */

.tabs {
    display: flex;
    justify-content: center;
    align-items: center;
    gap: 3rem;
    padding: 1rem;
    /* outline: 1px solid; */
}

.tabs__btn {
    font-size: 1.5rem;
    outline: none;
    transition: all 0.3s;
}

.tabs_btn--active {
    color: #7fe7c4;
    transform: scale(1.1);
}

/* Animation */

.v-enter-active,
.v-leave-active {
    transition: all 0.5s ease;
}

.v-enter-from,
.v-leave-to {
    transform: translateX(0.5rem);
    opacity: 0;
}

/* Transactions */

.history-income-tot {
    font-size: 1.8rem;
    font-weight: 200;
}

.bar-wrapper {
    height: 40vh;
    min-width: min(60vw, 40rem);
    /* max-width: 60vw; */
}

p {
    font-weight: 200;
    color: rgb(161, 159, 159);
}

span {
    display: inline-flexbox;
    font-weight: 400;
    font-size: 1rem;
    color: #97e1c4;
    align-self: center;
}
</style>