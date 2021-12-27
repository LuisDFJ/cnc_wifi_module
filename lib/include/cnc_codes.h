#ifndef CNC_CODES_H
#define CNC_CODES_H

#define BRUN_STATUS 0
#define BIT1_STATUS 1
#define BIT2_STATUS 2
#define BIT3_STATUS 3
#define BIT4_STATUS 4
#define BIT5_STATUS 5
#define BIT6_STATUS 6
#define BIT7_STATUS 7

#define SET_BIT( reg, bit ) \
    reg |= ( (unsigned char)1 << bit )

#define CLR_BIT( reg, bit ) \
    reg &= ~( (unsigned char)1 << bit )

#define REG_BIT( reg, bit ) \
    ( reg & ( (unsigned char)1 << bit ) )

#define DEC_REG( val, reg, macro )  \
    switch (val)                    \
    {                               \
    case BRUN_STATUS:               \
        macro( reg, BRUN_STATUS );  \
        ESP_LOGI( TAG, "1" );       \
        break;                      \
    case BIT1_STATUS:               \
        macro( reg, BIT1_STATUS );  \
        ESP_LOGI( TAG, "2" );       \
        break;                      \
    case BIT2_STATUS:               \
        macro( reg, BIT2_STATUS );  \
        ESP_LOGI( TAG, "3" );       \
        break;                      \
    case BIT3_STATUS:               \
        macro( reg, BIT3_STATUS );  \
        ESP_LOGI( TAG, "4" );       \
        break;                      \
    case BIT4_STATUS:               \
        macro( reg, BIT4_STATUS );  \
        ESP_LOGI( TAG, "5" );       \
        break;                      \
    case BIT5_STATUS:               \
        macro( reg, BIT5_STATUS );  \
        ESP_LOGI( TAG, "6" );       \
        break;                      \
    case BIT6_STATUS:               \
        macro( reg, BIT6_STATUS );  \
        ESP_LOGI( TAG, "7" );       \
        break;                      \
    case BIT7_STATUS:               \
        macro( reg, BIT7_STATUS );  \
        ESP_LOGI( TAG, "8" );       \
        break;                      \
    default:                        \
        break;                      \
    }                               \

#endif