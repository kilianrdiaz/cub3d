#include "../inc/cub3d.h"

//Busca una subcadena dentro de una cadena y devuelve 
char    *ft_strnstr(const char *s, const char *find, size_t len)
{
    size_t  i;
    size_t  j;
    i = 0;
    if (*find == '\0' || find == NULL)
        return ((char *)s);
    while (s[i] && i < len)
    {
        j = 0;
        while (s[i] == find[j] && s[i] && i < len)
        {
            i++;
            j++;
        }
        if (!find[j])
            return ((char *)&s[i - j]);
        i = (i - j) + 1;
    }
    return (0);
}

size_t  ft_strlen(const char *str)
{
    size_t  i;
    i = 0;
    while (str[i])
    {
        i++;
    }
    return (i);
}

char    *ft_strjoin(char const *s1, char const *s2)
{
        int             i;
        int             s1_len;
        int             s2_len;
        char    *str;

        if (!s1 || !s2)
                return (NULL);
        s1_len = ft_strlen(s1);
        s2_len = ft_strlen(s2);
        str = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
        if (!str)
                return (NULL);
        i = 0;
        while (s1_len--)
        {
                str[i] = s1[i];
                i++;
        }
        s1_len = i;
        i = 0;
        while (s2_len--)
                str[s1_len++] = s2[i++];
        str[s1_len] = '\0';
        return (str);
}