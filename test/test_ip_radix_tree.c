
#include <ip_radix_tree.h>

int main()
{
    ip_radix_tree_t *it;
    int_t            ret;

    it = ip_radix_tree_create();
    if (it == NULL) {
        printf("create error!\n");
        return 0;
    }

    //add 101.45.69.50/16
    ret = ip_radix_tree_insert(it, 1697465650, 0xffff0000, 1);
    if (ret != OK) {
        printf("insert 1 error.\n");
        goto error;
    }

    //add 10.45.69.50/16
    ret = ip_radix_tree_insert(it, 170738994, 0xffff0000, 1);
    if (ret != OK) {
        printf("insert 2 error.\n");
        goto error;
    }

    //add 10.45.79.50/16
    ret = ip_radix_tree_insert(it, 170741554, 0xffff0000, 1);
    if (ret == OK) {
        printf("insert 3 error.\n");
        goto error;
    }

    //add 102.45.79.50/24
    ret = ip_radix_tree_insert(it, 1714245426, 0xffffff00, 1);
    if (ret != OK) {
        printf("insert 4 error.\n");
        goto error;
    }

    ret = ip_radix_tree_find(it, 170741554);
    if (ret == 1) {
        printf("test case 1 passed\n");
    } else {
        printf("test case 1 error\n");
    }

    ret = ip_radix_tree_find(it, 170786817);
    if (ret != 1) {
        printf("test case 2 passed\n");
    } else {
        printf("test case 2 error\n");
    }

    ret = ip_radix_tree_delete(it, 1714245426, 24);
    if (ret != OK) {
        goto error;
    }
    
    ret = ip_radix_tree_find(it, 1714245426);
    if (ret != 1) {
        printf("test case 3 passed\n");
    } else {
        printf("test case 3 error\n");
    }
    
 error:
    ip_radix_tree_destroy(it);
    return 0;
}
