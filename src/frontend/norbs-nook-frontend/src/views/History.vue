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
import { Debug, Information, WarningAlt, ErrorOutline } from '@vicons/carbon';

// Transaction Related Data

ChartJS.register(CategoryScale, LinearScale, BarElement, LineElement, PointElement, Title, Tooltip, Legend);

// const activeTab = ref('transactions');
const tabActive = ref({
    transactions: true,
    workerReport: false,
    logs: false
})
const setActiveTab = (tab: string) => {
    Object.keys(tabActive.value).forEach(key => {
        tabActive.value[key] = false;
    });
    // Add a delay of 0.52s to ensure the transition is complete
    setTimeout(() => {
        tabActive.value[tab] = true;
    }, 530);
}

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

const fetchTransactionsData = async () => {
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
            labels: response.data.content.map((_: any, i: any) => `Transaction-${i + 1}`),
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

// Worker Report Related Data
const workerReportData = ref<any>(null);
const fetchWorkerReport = async () => {
    const response = await axios.post("http://localhost:5000/api/get_worker_report");
    console.log(response.data);
    if (response.data.status == 'success') {
        workerReportData.value = response.data.content;
    } else {
        message.error(response.data.message);
    }
}

// Logs Related Data
const logsData = ref<any>(null);
const fetchLogs = async () => {
    const response = await axios.post("http://localhost:5000/api/get_logs");
    console.log(response.data);
    if (response.data.status == 'success') {
        // Turn each entry from a string to an object
        const processedData = response.data.content.map((item: string) => {
            let type = item.split(' ')[0];
            type = type.slice(1, type.length - 1);
            const content = item.slice(type.length + 2);
            return {
                mode: type,
                content: content
            }
        });
        console.log('processedData:', processedData);
        logsData.value = processedData;
    } else {
        message.error(response.data.message);
    }
}
const logColors = {
    DEBUG: '#7fe7c4',
    INFO: '#70c0e8',
    WARNING: '#f9d71c',
    ERROR: '#f97e7e'
}
const levelGreaterThan = (current: string, target: string) => {
    const levels = ['DEBUG', 'INFO', 'WARNING', 'ERROR'];
    return levels.indexOf(current) >= levels.indexOf(target);
}
const logModeRef = ref('INFO');

onMounted(async () => {
    chartData.value = await fetchTransactionsData();;
    // @ts-ignore  < false alarm by vscode >
    if (chartRef.value) {
        chartRef.value.renderChart(chartData.value, chartOptions.value);
    }
    fetchWorkerReport();
    fetchLogs();
});
</script>

<template>
    <div class="history-container a-fade-in">
        <div class="history-container">
            <div class="tabs">
                <n-tooltip trigger="hover">
                    <template #trigger>
                        <n-button text class="tabs__btn tabs__btn--transactions" @click="setActiveTab('transactions')"
                            :class="{ 'tabs_btn--active': tabActive.transactions }">
                            <n-icon>
                                <ChartLine />
                            </n-icon>
                        </n-button>
                    </template>
                    Transactions
                </n-tooltip>
                <n-tooltip trigger="hover">
                    <template #trigger>
                        <n-button text class="tabs__btn tabs__btn--worker-report" @click="setActiveTab('workerReport')"
                            :class="{ 'tabs_btn--active': tabActive.workerReport }">
                            <n-icon>
                                <UserData />
                            </n-icon>
                        </n-button>
                    </template>
                    Worker Report
                </n-tooltip>
                <n-tooltip trigger="hover">
                    <template #trigger>
                        <n-button text class="tabs__btn tabs__btn--logs" @click="setActiveTab('logs')"
                            :class="{ 'tabs_btn--active': tabActive.logs }">
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
                    <div class="transition-body transactions-body" v-if="tabActive.transactions">
                        <!-- <canvas id="transactionChart"></canvas> -->
                        <!-- <ChartComponent :chart-data="chartData" :chart-options="chartOptions" /> -->
                        <div class="transaction-wrapper" v-if="chartResponded && chartData">
                            <h1 class="history-income-tot">
                                Transactions
                            </h1>
                            <div class="bar-wrapper">
                                <Bar :data="chartData" :options="chartOptions" />
                            </div>
                            <p class="chart-info__basic">{{ chartExtract?.purchases }} purchases, {{
                                chartExtract?.imports }}
                                imports are recorded
                                in this
                                season.</p>
                            <p class="chart-info__basic">Total revenue generated: <span
                                    class="chart-info__highlight">${{
                                        chartExtract?.revenue }}</span></p>
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
                <Transition>
                    <div class="transition-body worker-report-body" v-if="tabActive.workerReport">
                        <div v-if="workerReportData">
                            <h1 class="history-income-tot">
                                Worker Report
                            </h1>
                            <n-scrollbar style="max-height: 50vh; min-width: 50vw;" trigger="none">
                                <div v-for="(worker, i) in workerReportData" :key="i" class="worker-report-wrapper">
                                    <n-flex gap="1rem" align="center">
                                        <n-avatar round>{{ worker.user_id[0].toUpperCase() }}</n-avatar>
                                        <div class="message-text">
                                            <h3 class="message-text__user-id">{{ worker.user_id }}</h3>
                                            <p class="message-text__description">{{ worker.description }}</p>
                                        </div>
                                    </n-flex>
                                </div>
                            </n-scrollbar>
                        </div>
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
                <Transition>
                    <div class="transition-body logs-body" v-if="tabActive.logs">
                        <div v-if="logsData">
                            <h1 class="history-income-tot">
                                Logs
                            </h1>
                            <n-space vertical size="large">
                                <n-radio-group size="small" v-model:value="logModeRef">
                                    <n-radio-button value="DEBUG">Debug</n-radio-button>
                                    <n-radio-button value="INFO">Info</n-radio-button>
                                    <n-radio-button value="WARNING">Warning</n-radio-button>
                                    <n-radio-button value="ERROR">Error</n-radio-button>
                                </n-radio-group>
                                <n-scrollbar style="max-height: 40vh; min-width: 50vw;" trigger="none">
                                    <TransitionGroup>
                                        <div v-for="(log, i) in logsData.filter((item: any) => levelGreaterThan(item.mode, logModeRef))"
                                            :key="i" class="log-report-wrapper">
                                            <n-flex gap="1rem" align="center">
                                                <n-avatar round
                                                    :style="{ 'background-color': logColors[log.mode], color: '#1c1b1b' }">
                                                    <n-icon v-if="log.mode === 'DEBUG'" size="large">
                                                        <Debug />
                                                    </n-icon>
                                                    <n-icon v-else-if="log.mode === 'INFO'" size="large">
                                                        <Information />
                                                    </n-icon>
                                                    <n-icon v-else-if="log.mode === 'WARNING'" size="large">
                                                        <WarningAlt />
                                                    </n-icon>
                                                    <n-icon v-else-if="log.mode === 'ERROR'" size="large">
                                                        <ErrorOutline />
                                                    </n-icon>
                                                </n-avatar>
                                                <div class="message-text">
                                                    <h3 class="message-text__log-id"
                                                        :style="{ 'color': logColors[log.mode] }">
                                                        {{ log.mode
                                                        }}</h3>
                                                    <p class="message-text__description">{{ log.content }}</p>
                                                </div>
                                            </n-flex>
                                        </div>
                                    </TransitionGroup>
                                </n-scrollbar>
                            </n-space>
                        </div>
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

.main-body {
    position: relative;
}

.transition-body {
    /* position: absolute; */
    top: 0;
    left: 0;
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

.v-enter-from {
    transform: translateX(0.5em);
    opacity: 0;
}

.v-leave-to {
    transform: translateX(-0.5rem);
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

.chart-info__basic {
    font-weight: 200;
    color: rgb(161, 159, 159);
}

.chart-info__highlight {
    display: inline-flexbox;
    font-weight: 400;
    font-size: 1rem;
    color: #97e1c4;
    align-self: center;
}

/* Worker Report */

.worker-report-wrapper,
.log-report-wrapper {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 0.5rem;
}

.message-text {
    display: flex;
    flex-direction: column;
    align-items: flex-start;
    /* gap: 0.2rem; */
}

.message-text__log-id,
.message-text__user-id {
    font-size: 1.2rem;
    font-weight: 200;
    color: #7fe7c4;
    margin: 0;
}

.message-text__description {
    font-size: 0.8rem;
    font-weight: 200;
    color: rgb(161, 159, 159);
    margin: 0;
}

/* Logs */
</style>