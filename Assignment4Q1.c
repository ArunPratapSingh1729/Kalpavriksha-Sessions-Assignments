#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product
{
    int Id;
    char Name[20];
    int Price;
    int Quantity;
};

int isValidProductName(const char *name)
{
    for (int i = 0; name[i] != '\0'; i++)
    {
        if (!isalnum(name[i]) && name[i] != '-')
            return 0;
    }
    return 1;
}

int validateProduct(struct Product p)
{
    if (p.Id < 1 || p.Id > 10000)
    {
        printf("Error: Product ID must be between 1 and 10000.\n");
        return 0;
    }

    if (strlen(p.Name) < 1 || strlen(p.Name) > 50)
    {
        printf("Error: Product name length must be between 1 and 50 characters.\n");
        return 0;
    }

    if (p.Price < 0 || p.Price > 100000)
    {
        printf("Error: Price must be between 0 and 100000.\n");
        return 0;
    }
    if (p.Quantity < 0 || p.Quantity > 1000000)
    {
        printf("Error: Quantity must be between 0 and 1000000.\n");
        return 0;
    }
    return 1;
}

int findProductById(struct Product *arr, int count, int id)
{
    for (int i = 0; i < count; i++)
    {
        if (arr[i].Id == id)
            return i;
    }
    return -1;
}

void addProduct(struct Product **arr, int *count)
{
    struct Product p;
    int valid = 0;

    while (!valid)
    {
        printf("Enter new product details:\n");
        printf("Product ID: ");
        scanf("%d", &p.Id);
        printf("Product Name: ");
        scanf("%s", p.Name);
        printf("Product Price: ");
        scanf("%f", &p.Price);
        printf("Product Quantity: ");
        scanf("%d", &p.Quantity);

        valid = validateProduct(p);

        for (int i = 0; i < count; i++)
        {
            if (arr[i]->Id == p.Id)
            {
                printf("Product Id already exists \n");
                valid = 0;
            }
        }

        if (findProductById(*arr, *count, p.Id) != -1)
        {
            printf("Error: Product ID already exists.\n");
            continue;
        }

        valid = validateProduct(p);
        if (!valid)
            printf("Please re-enter valid product details.\n\n");
    }

    struct Product *temp = realloc(*arr, (*count + 1) * sizeof(struct Product));
    if (temp == NULL)
    {
        printf("Memory Allocation Failed \n");
        free(*arr);
        exit(1);
    }

    *arr = temp;
    (*arr)[*count] = p;
    (*count)++;
    printf("Product Added Successfully \n");
}

void viewProducts(struct Product *arr, int count)
{

    if (count == 0)
    {
        printf("No products available.\n");
        return;
    }

    printf("========= PRODUCT LIST =========\n");
    for (int i = 0; i < count; i++)
    {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               arr[i].Id, arr[i].Name, arr[i].Price, arr[i].Quantity);
    }
}

void updateQuantity(struct Product *arr, int count)
{
    int Id, newQuantity, found = 0;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &Id);

    int index = findProductById(arr, count, id);
    if (index == -1)
    {
        printf("Product with ID %d not found.\n", id);
        return;
    }

    for (int i = 0; i < count; i++)
    {
        if (arr[i].Id == Id)
        {
            printf("Enter new Quantity: ");
            scanf("%d", &newQuantity);

            if (newQuantity < 0 || newQuantity > 1000000)
            {
                printf("Error: Quantity must be between 0 and 1000000.\n");
                return;
            }

            arr[i].Quantity = newQuantity;
            printf("Quantity updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Product with ID %d not found.\n", Id);
    }
}

void searchByID(struct Product *arr, int count)
{
    int id, found = 0;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++)
    {
        if (arr[i].Id == id)
        {
            printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   arr[i].Id, arr[i].Name, arr[i].Price, arr[i].Quantity);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Product with ID %d not found.\n", id);
}

void searchByName(struct Product *arr, int count)
{
    char name[50];
    int found = 0;
    
    while(1){

        printf("Enter name to search (partial allowed): ");
        scanf("%s", name);
        
        if (!isValidProductName(name))
        {
            printf("Enter the correct product name \n");
            continue;
        }
        else{
            printf("Products Found:\n");
            break;
        }
    }
    
    for (int i = 0; i < count; i++)
    {
        if (strstr(arr[i].Name, name) != NULL)
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   arr[i].Id, arr[i].Name, arr[i].Price, arr[i].Quantity);
            found = 1;
        }
    }

    if (!found)
        printf("No products found with name containing '%s'.\n", name);
}

void searchByPriceRange(struct Product *arr, int count)
{

    float min, max;
    int found = 0;
    printf("Enter the min value \n");
    scanf("%d", &min);
    printf("Enter the max value \n");
    scanf("%d", &max);

    if (min < 0 || max > 1000 || min > max)
    {
        printf("Error: Enter valid price range between 0 and 100000.\n");
        return;
    }
    printf("Products in price range:\n");

    for (int i = 0; i < count; i++)
    {
        if (arr[i].Price >= min && arr[i].Price <= max)
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   arr[i].Id, arr[i].Name, arr[i].Price, arr[i].Quantity);
            found = 1;
        }
    }
    if (!found)
        printf("No products found in this price range.\n");
}

void deleteById(struct Product **arr, int *count)
{
    int Id, found = 0;
    printf("Enter the Product Id to delete: \n");
    scanf("%d", &Id);

    for (int i = 0; i < *count; i++)
    {
        if ((*arr)[i].Id == Id)
        {
            for (int j = i; j < *count; j++)
            {
                arr[j] = arr[j + 1];
                (*count)--;
                *arr = realloc(*arr, *count * sizeof(struct Product));
                printf("Product Deleted Successfully \n");
                found = 1;
                break;
            }
        }
    }
    if (!found)
        printf("Product with ID %d not found.\n", Id);
}

int main()
{
    struct Product *product = NULL;
    int count = 0;
    int initial;

    printf("Enter initial number of products: ");
    scanf("%d", &initial);

    if (initial < 1 || initial > 100)
    {
        printf("Error: Initial number of products must be between 1 and 100.\n");
        return 1;
    }

    product = calloc(initial, sizeof(struct Product));
    if (product == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    count = initial;

    for (int i = 0; i < initial; i++)
    {

        int choice;
        while (1)
        {
            printf("\n========= INVENTORY MENU =========\n");
            printf("1. Add New Product\n");
            printf("2. View All Products\n");
            printf("3. Update Quantity\n");
            printf("4. Search Product by ID\n");
            printf("5. Search Product by Name\n");
            printf("6. Search Product by Price Range\n");
            printf("7. Delete Product\n");
            printf("8. Exit\n");
            printf("==================================\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                addProduct(&product, &count);
                break;
            case 2:
                viewProducts(product, count);
                break;
            case 3:
                updateQuantity(product, count);
                break;
            case 4:
                searchByID(product, count);
                break;
            case 5:
                searchByName(product, count);
                break;
            case 6:
                searchByPriceRange(product, count);
                break;
            case 7:
                deleteByID(&product, &count);
                break;
            case 8:
                printf("Exiting program...\n");
                free(product);
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
            }
        }

        return 0;
    }
