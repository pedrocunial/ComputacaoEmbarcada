#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROD_NAME_SIZE 15

const char *product_name = "Nome do Produto";
const char *unit_price = "Preço Unitário";
const char *bought_qtd = "Quantidade Comprada";
const char *product_total = "Total do Produto";

typedef struct product
{
    char name[20];
    unsigned short qtd;
    float price;
} product_t;


product_t * create_table(product_t *products, unsigned short n_products)
{

    for (n_products; n_products--;) {
        printf("Entre o nome do produto:\n");
        scanf("%s", &products[n_products].name);
        printf("Entre o preco unitario do produto:\n");
        scanf("%f", &products[n_products].price);
        printf("Entre a quantidade comprada:\n");
        scanf("%d", &products[n_products].qtd);
    }
    return products;
}

void show_table(product_t *products, unsigned short n_products) {
    printf("| %s | %s | %s | %s |\n",
           product_name,
           unit_price,
           bought_qtd,
           product_total);

    for (n_products; n_products--;) {
        char prod_name_lenght = strlen(products[n_products].name);
        prod_name_lenght = PROD_NAME_SIZE - prod_name_lenght;
        if (prod_name_lenght < 0) {
            prod_name_lenght = 0;
            products[n_products].name[PROD_NAME_SIZE] = 0;
        }

        printf("| %s ", products[n_products].name);
        for (prod_name_lenght; prod_name_lenght--;)
            printf(" ");

        printf("| %4.2f           | %4.2d                | %4.2f             |\n",
               products[n_products].price,
               products[n_products].qtd,
               products[n_products].price * products[n_products].qtd);
    }
}

int main()
{
    unsigned short n_products;
    product_t *products;

    printf("Quantas comidas você comprou?\n");
    scanf("%d", &n_products);
    products = malloc(n_products * sizeof(product_t));
    show_table(create_table(products, n_products), n_products);

    return 0;
}
