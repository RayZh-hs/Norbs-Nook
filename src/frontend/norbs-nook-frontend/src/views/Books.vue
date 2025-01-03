<script lang="ts">
import { ref, onMounted, watchEffect } from 'vue';
import SearchBar from '../components/SearchBar.vue';
import BookEditCard from '../components/BookEditCard.vue';
import { useRoute, useRouter } from 'vue-router';
import { reactive } from 'vue';
import { useMessage } from 'naive-ui';
import axios from 'axios';
import { TransitionGroup } from 'vue';
import { ShoppingCart, Edit, ImportExport } from '@vicons/carbon';
import type { FormInst } from 'naive-ui'
import { BookSharp } from '@vicons/ionicons5';

export default {
    name: 'Books',
    components: { SearchBar, ShoppingCart, Edit, ImportExport },
    setup() {

        const message = useMessage();

        const searchData = ref<any | null>(null);
        const searchQuery = ref('');
        const route = useRoute();
        const router = useRouter();
        const isLoading = ref(true);
        const books = ref([]);
        const activeBookNumber = ref(0);
        const account_info = ref({ username: 'Guest' });

        const showBookInfo = ref(false);
        const showPayment = ref(false);
        const showImport = ref(false);
        const showEdit = ref(false);
        const showAddBook = ref(false);

        const buy_amount = ref(1);

        const importFormRef = ref<FormInst | null>(null);
        const importForm = reactive({
            quantity: '',
            price: ''
        });
        const importFormRules = {
            quantity: {
                required: true,
                message: 'Please enter the quantity of books imported',
            },
            price: {
                required: true,
                message: 'Please enter the total price of the import',
            },
        };

        const fetchData = async () => {
            isLoading.value = true; // Start loading
            try {
                const searchDataString = localStorage.getItem('search_data');
                console.log('Search data:', searchDataString);
                if (searchDataString) {
                    searchData.value = JSON.parse(searchDataString);
                    searchQuery.value = (
                        () => {
                            if (searchData.value?.query_mode === 'all') { // Added optional chaining
                                return 'all';
                            } else if (searchData.value?.query_mode === 'keywords') { // Added optional chaining
                                return searchData.value.query_tags?.join(', ') || ''; // Added optional chaining and default value
                            } else {
                                return searchData.value?.query_content || ''; // Added optional chaining and default value
                            }
                        }
                    )();
                }
            } catch (error) {
                console.error('Error parsing search data:', error);
            } finally {
                isLoading.value = false; // End loading
            }
        };

        const fetchAccount = () => {
            // Fetch from local storage
            const accountInfoString = localStorage.getItem('account_info');
            if (accountInfoString) {
                account_info.value = JSON.parse(accountInfoString);
            }
        }

        onMounted(fetchData, fetchAccount); //Initial fetch on mount

        // This function calculates the string to be displayed.  No change here
        const resString = ref(() => {
            if (searchData.value?.query_mode === 'all') { // Added optional chaining
                return 'all';
            } else if (searchData.value?.query_mode === 'keywords') { // Added optional chaining
                return searchData.value.query_tags?.join(', ') || ''; // Added optional chaining and default value
            } else {
                return searchData.value?.query_content || ''; // Added optional chaining and default value
            }
        });

        // The api call to get all the books that match the search query
        const handleSubmitQuery = async (searchData: any) => {
            console.log('Posting query to the api:', searchData);
            const response = await axios.post('http://localhost:5000/api/find_books', searchData);
            console.log('Response:', response.data);
            // Update the books array with the response data
            if (response.data.status === 'success') {
                books.value = response.data.content;
            }
            else {
                books.value = [];
                // Send a warning message to the user
                message.error(response.data.message);
            }
        };

        const handleBuy = async () => {
            console.log(books.value, activeBookNumber.value)
            console.log('Buying book:', books.value[activeBookNumber.value]);
            const response = await axios.post('http://localhost:5000/api/buy_book', {
                isbn: books.value[activeBookNumber.value].isbn,
                amount: buy_amount.value
            });
            console.log('Response:', response.data);
            if (response.data.status === 'success') {
                // Send a success message to the user
                message.success(response.data.message);
                // Refresh the list of books
                handleSubmitQuery(searchData.value);
            }
            else {
                // Send a warning message to the user
                message.error(response.data.message);
            }
        };

        const handleImport = async () => {
            console.log('Importing book:', books.value[activeBookNumber.value]);
            const response = await axios.post('http://localhost:5000/api/import_book', {
                isbn: books.value[activeBookNumber.value].isbn,
                amount: importForm.quantity,  // This is a string
                cost: importForm.price         // This is a string too
            });
            console.log('Response:', response.data);
            if (response.data.status === 'success') {
                // Send a success message to the user
                message.success(response.data.message);
                // Refresh the list of books
                handleSubmitQuery(searchData.value);
            }
            else {
                // Send a warning message to the user
                // In this case, the message may be retrieved from the python validator
                message.error(response.data.message);
            }
        };

        const handleModified = () => {
            message.success('Book modified successfully');
            // Refresh the list of books
            handleSubmitQuery(searchData.value);
        };

        const handleSearch = (search_data: any) => {
            console.log('Search data:', search_data);
            // Save the data to local storage:
            localStorage.setItem('search_data', JSON.stringify(search_data));
            // Refresh the list of books
            handleSubmitQuery(search_data);
            searchQuery.value = (
                () => {
                    if (search_data.query_mode === 'all') { // Added optional chaining
                        return 'all';
                    } else if (search_data.query_mode === 'keywords') { // Added optional chaining
                        return search_data.query_tags?.join(', ') || ''; // Added optional chaining and default value
                    } else {
                        return search_data.query_content || ''; // Added optional chaining and default value
                    }
                }
            )();
        };

        const handleAddedNewBook = () => {
            showAddBook.value = false;
            message.success('Book added!');
            // Refresh the list of books
            handleSubmitQuery(searchData.value);
        };

        // Watch for changes in the route (Composition API style)
        watchEffect(() => {
            console.log("Route changed", route.params);
            fetchData();
            fetchAccount();
            handleSubmitQuery(searchData.value);
        });

        return {
            searchData, searchQuery, isLoading, resString, books,
            showBookInfo, activeBookNumber, buy_amount, account_info,
            showPayment, handleBuy, showImport, showEdit, importForm,
            importFormRef, importFormRules, handleImport, handleModified,
            handleSearch, showAddBook, handleAddedNewBook
        };
    },
};
</script>

<template>
    <div class="main-books-container">
        <div class="data-available-container" v-if="searchData">
            <SearchBar :query_mode="searchData.query_mode" :query_input="searchData.query_content"
                :query_tags="searchData.query_tags" class="a-trans-2" @search="handleSearch" />
            <n-divider class="a-trans-2"></n-divider>
            <h2 class="a-trans-2">{{ books.length }} Search results for "{{ searchQuery }}"</h2>
            <n-scrollbar id="book-list">
                <TransitionGroup name="list" tag="div">
                    <div v-for="(book, serial) in books" :key="book.isbn">
                        <div class="book-brief-card" @click="activeBookNumber = serial; showBookInfo = true;">
                            <!-- <div class="book-brief-card__main-sect"> -->
                            <h3 class="book-brief-card__title">{{ book.name }}</h3>
                            <div class="book-brief-card__tags">
                                <n-tag v-for="keyword in book.keyword" type="info" class="book-brief-card__tag">
                                    {{ keyword }}
                                </n-tag>
                            </div>
                            <!-- </div> -->
                            <div class="book-brief-card__appended-sect">
                                <n-tag v-if="book.quantity == 0" type="error">Out of stock</n-tag>
                                <span v-else class="book-brief-card__available-text">
                                    ${{ book.price }} / {{ book.quantity }} left
                                </span>
                            </div>
                        </div>
                    </div>
                </TransitionGroup>
                <n-button text class="add-book-btn" v-if="account_info.content.privilege >= 3"
                    @click="showAddBook = true;"
                >
                    No results? Add new book here.
                </n-button>
            </n-scrollbar>
        </div>
        <h2 v-else>Loading search results...</h2>
        <n-modal v-model:show="showBookInfo" @close="showBookInfo = false">
            <n-card class="book-detailed-card card-color-override">
                <div class="book-detailed-card__general-info">
                    <n-flex align="center">
                        <div class="book-detailed-card__cover-placeholder">
                            Cover NA
                        </div>
                        <n-flex vertical justify="space-between" style="max-width: 16rem;">
                            <h2 style="margin-top: 0">{{ books[activeBookNumber].name }}</h2>
                            <p>author: {{ books[activeBookNumber].author }}</p>
                            <p>isbn: {{ books[activeBookNumber].isbn }}</p>
                            <p>keywords: {{ books[activeBookNumber].keyword.join(', ') }}</p>
                            <p>price: ${{ books[activeBookNumber].price }}</p>
                            <p>quantity: {{ books[activeBookNumber].quantity }}</p>
                        </n-flex>
                    </n-flex>
                </div>
                <n-divider />
                <div class="book-detailed-card__description">
                    <h3>Description</h3>
                    <p>{{ books[activeBookNumber].description
                        ? books[activeBookNumber].description
                        : 'Description not available yet.' }}</p>
                </div>
                <div class="book-detailed-card__actions">
                    <n-flex>
                        <n-tooltip trigger="hover" v-if="account_info.content.privilege >= 3">
                            <template #trigger>
                                <n-button type="default" style="margin-right: 0.5rem;" @click="showEdit = true">
                                    <n-icon size="large">
                                        <Edit />
                                    </n-icon>
                                </n-button>
                            </template>
                            <span>Edit</span>
                        </n-tooltip>
                        <n-tooltip trigger="hover" v-if="account_info.content.privilege >= 3">
                            <template #trigger>
                                <n-button type="default" style="margin-right: 0.5rem;" @click="showImport = true">
                                    <n-icon size="large">
                                        <ImportExport />
                                    </n-icon>
                                </n-button>
                            </template>
                            <span>Import</span>
                        </n-tooltip>
                        <n-input-number v-model:value="buy_amount" :min="1" :max="books[activeBookNumber].quantity"
                            style="max-width: 8rem;" :disabled="books[activeBookNumber].quantity <= 0" />
                        <n-button type="primary" :disabled="books[activeBookNumber].quantity <= 0"
                            @click="showPayment = true">
                            <n-icon size="large" style="margin-right: 0.5rem;">
                                <ShoppingCart />
                            </n-icon>
                            Buy Now
                        </n-button>
                    </n-flex>
                </div>
            </n-card>
        </n-modal>
        <n-modal v-model:show="showPayment" :mask-closable="false">
            <n-card :title="'Scan to pay $' + (buy_amount * books[activeBookNumber].price).toFixed(2)"
                class="card-color-override payment-card">
                <n-flex vertical justify="center" align="center">
                    <n-qr-code :value="'You paid $' + (buy_amount * books[activeBookNumber].price).toFixed(2)"
                        :size="160" style="width: 90%; height: 80%;"></n-qr-code>
                    <n-flex style="margin-top: 0.5rem;">
                        <n-button type="default" @click="showPayment = false">Cancel</n-button>
                        <n-button type="default" @click="showPayment = false; handleBuy()">Confirm</n-button>
                    </n-flex>
                </n-flex>
            </n-card>
        </n-modal>
        <n-modal v-model:show="showImport">
            <n-card class="card-color-override import-card" title="Import" style="text-align: center;">
                <n-flex vertical justify="center" align="center">
                    <n-form :model="importForm" :rules="importFormRules" ref="importFormRef">
                        <n-form-item label="Quantity" path="quantity" style="min-width: 18rem;">
                            <n-input type="number" v-model:value="importForm.quantity"
                                placeholder="How many did you import?" />
                        </n-form-item>
                        <n-form-item type="number" label="Price" path="price">
                            <n-input v-model:value="importForm.price" placeholder="How much did you spend?">
                                <template #suffix>$</template>
                            </n-input>
                        </n-form-item>
                    </n-form>
                    <n-flex style="margin-top: 0.5rem;">
                        <n-button type="default" @click="showImport = false">Cancel</n-button>
                        <n-button type="default" @click="showImport = false; handleImport();">Confirm</n-button>
                    </n-flex>
                </n-flex>
            </n-card>
        </n-modal>
        <n-modal v-model:show="showEdit">
            <BookEditCard :isbn="books[activeBookNumber].isbn" :title="books[activeBookNumber].name"
                :author="books[activeBookNumber].author" :keywords="books[activeBookNumber].keyword"
                :price="books[activeBookNumber].price" cardTitle="Edit Book"
                @modify="showEdit = false; handleModified();" @close="showEdit = false;"
                :original_isbn="books[activeBookNumber].isbn" onIsbnCollide="ignore" />
        </n-modal>
        <n-modal v-model:show="showAddBook">
            <BookEditCard onIsbnCollide="abort" cardTitle="New Book" @close="showAddBook = false"
                @modify="handleAddedNewBook" />
        </n-modal>
    </div>
</template>

<style scoped>
.card-color-override {
    background-color: #262626;
}

.n-modal {
    z-index: auto !important;
    /* Ensure modals stack correctly */
}

.main-books-container {
    min-width: 30rem;
}

.search-bar {
    margin-top: 2pt;
}

.list-enter-active,
.list-leave-active {
    transition: all 0.5s ease;
}

.list-enter-from,
.list-leave-to {
    opacity: 0;
    transform: translateX(30px);
}

.book-list {
    max-height: 50vh;
    width: 120%;
    left: -10%;
    /* max-width: 90vw; */
}

.book-brief-card {
    display: flex;
    width: 100%;
    height: 4rem;
    margin-bottom: 1rem;
    align-items: center;
    /* border: 1px solid #e0e0e0;
    border-radius: 0.5rem; */
    border-radius: 7px;
    background: #202020;
    box-shadow: 5px 5px 10px #1a1a1a,
        -5px -5px 10px #262626;
    transition: transform 0.2s;
    cursor: pointer;
}

.book-brief-card:hover {
    transform: scale(1.02);
}

.book-brief-card__title {
    margin: 1rem;
    font-size: 1.4rem;
    font-weight: 200;
}

.book-brief-card__tags {
    display: flex;
    flex-wrap: wrap;
    gap: 0.5rem;
    margin: 1rem;
}

.book-brief-card__tag {
    height: 1.2rem;
    width: auto;
    font-size: 0.6rem;
    font-weight: 200;
    transition: transform 0.2s;
}

.book-brief-card__tag:hover {
    transform: scale(1.1);
    /* Make it a bit brighter too */
    filter: brightness(1.2);
}

.book-brief-card__appended-sect {
    position: relative;
    right: 0;
    display: flex;
    justify-self: flex-end;
    margin-left: auto;
    text-align: right;
    margin-right: 1rem;
    font-size: 0.8rem;
    font-weight: 200;
}

.book-brief-card__available-text {
    font-size: 0.95rem;
    color: #63e2b7;
}

.book-detailed-card {
    max-width: 35rem;
    min-width: 20rem;
    min-height: 40rem;
    max-height: min(45rem, 80vh);
    display: flexbox;
    flex-direction: column;
    justify-content: space-between;
    align-content: center;
}

.book-detailed-card__general-info {
    display: flex;
    flex-direction: column;
    justify-content: space-between;
    align-content: center;
    padding: 1rem;
}

.book-detailed-card__cover-placeholder {
    width: 10rem;
    height: 15rem;
    border-radius: 0.5rem;
    margin-right: 1rem;
    border: 1px dashed #e0e0e0;
    display: flex;
    justify-content: center;
    align-items: center;
    font-size: 1.5rem;
    font-weight: 200;
    margin-right: 2.5rem;
}

p {
    padding: 0;
    margin: 0;
}

.book-detailed-card__description {
    padding: 0 1.2rem;
}

.book-detailed-card__actions {
    position: absolute;
    right: 3rem;
    bottom: 2rem;
}

.payment-card {
    width: 16rem;
    height: 20rem;
}

.import-card {
    width: 24rem;
}

.add-book-btn {
    color: #a6a6a6;
    margin-top: 2rem;
    margin-left: 1rem;
}
</style>