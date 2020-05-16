## std::vector funkcijų atkartojimas vector klasėje

#### funkciju aprasymas:
1. emplace - įdeda nurodytą elementą į nurodytą vietą
2. emplace_back - įdeda nurodytą elementą i galą
3. swap - sukeicia dviejų veiktorių duomenis
4. pop_back - panaikina paskutinį elementą
5. push_back - įdeda reikšmę į vektoriaus galą

### spartos analizė
#### My PC :
1. CPU - Intel core i7
2. RAM - 8GB
3. HDD - 1TB
 
|            |    10k    |    100k   |   1mln   |   10mln |  100mln |
|------------|-----------|-----------|----------|---------|---------|
|std::vector | 0.0037784 | 0.0443535 | 0.386238 | 3.81299 | 31.8613 |
|class vector| 0.0024196 | 0.0191371 | 0.194752 | 2.83297 | 16.2168 |

### spartos analizė su studentų 100K failu

|            | Skaitymas |Skirstymas |
|------------|-----------|-----------|
|std::vector | 0.441157  |  1.21694  | 
|class vector| 0.55822   |  0.623789 |

