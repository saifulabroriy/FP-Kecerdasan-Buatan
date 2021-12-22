#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <array>
#include <ctime>
#include <cmath>
#include <string.h> 
using namespace std;

static int papan_goal[5][5], papan_acak[5][5];	// inisialisasi papan
static int indeks_blank[2]; 					// indeks_blank[0] = baris, [1] = kolom			// untuk menandai posisi 0 pada papan
static int rute[200000000];						// rute yang akan diambil selama pencarian
static int indeks_rute = 0;						// untuk menandai posisi terakhir node berada
static int jumlah_node;

int acak_papan();
int cetak_papan(int papan[5][5]);
int cari_nilai(int nilai);							// untuk mencari posisi yang seharusnya dari sebuah nilai
int manhattan_distance(int papan[5][5]);
int cek_gerakan(int gerakan);						// untuk memastikan gerakan tidak keluar dari papan
int gerak(int gerakan);								// untuk menggerakkan 0 pada papan acak
int astar(int heuristik, int &cost);							// untuk menentukan gerakan mana yang akan diambil parameter 0 = manhattan, parameter 1 = herustik buatan sendiri
int generate_id(int papan[5][5]);					// untuk membuat id khusus yang digunakan untuk menandai apakah papan berulang
int cek_rute(int id);								// untuk mengecek apakah rute yang akan dilewati sama dengan sebelumnya
int custom_distance(int papan[5][5]);				// nilai heuristik buatan sendiri (Menghitung banyaknya posisi puzzle yang benar)
double effective_branching_factor(int kedalaman);	// untuk menghitung nilai BFS dari searching yang telah dilakukan

main(){
	bool on = true;			// sebagai parameter berjalannnya program, bila false program akan menutup
	int menu, isi = 1;		// menu untuk menampung pilihan user, isi digunakan untuk mengisi papan goal
	int nilai_manhattan;
	int nilai_custom;		// nilai dari heuristik buatan sendiri
	int indeks_gerakan;		// untuk menentukan gerakan 0. 0=atas, 1=bawah, 2=kanan, 3=kiri
	char gerakan[6];		// untuk menampilkan gerakan. atas, bawah, dsb
	int kedalaman = 0;		// untuk menghitung kedalaman pencarian pada searching tree
	
	for(int baris = 0; baris<5; baris++){		//isi papan goal
		for(int kolom = 0; kolom<5; kolom++){
			if(isi == 25) isi = 0;				//sel pojok kanan bawah dikosongi
			papan_goal[baris][kolom] = isi;
			isi++;
		}
	}
	
	while(on == true){
		memset(rute, 0, sizeof rute);	// membersihkan rute searching
		indeks_rute = 0;				// mengembalikan posisi penunjuk akhir rute ke awal
		clock_t total_waktu = 0;		// untuk mengukur seberapa lama algoritma berjalan
		clock_t timer_mulai = 0;
		clock_t timer_selesai = 0;
		jumlah_node = 0;				// jumlah node yang ada dalam searching tree
		kedalaman = 0;					// kedalaman goals dalam tree
		total_waktu = 0;
		int cost = 0; 					// nilai G(n)
		
		system("CLS");
		printf("\t\t----------------\n\t\t24-PUZZLE SOLVER\n\t\t----------------\n\t\t\t      \n\n");														//tampilkan menu
		printf("1. Algoritma A* dengan Manhattan Distance\n");
		printf("2. Algoritma A* dengan heuristik buatan sendiri\n");
		printf("0. Keluar \n\n");
		printf("Pilih metode pencarian : ");
		scanf("%d", &menu);
		
		switch(menu){
			case 1 :
				acak_papan();										// setiap memulai ulang pencarian papan selalu diacak secara otomatis
				system("CLS");
				printf("\nPENCARIAN A* DENGAN MANHATTAN DISTANCE\n");
				printf("\n==== Initial Board Configuration ====\n\n");
				cetak_papan(papan_acak);
				printf("\n=====================================\n");
				nilai_manhattan = manhattan_distance(papan_acak);	// cek nilai manhattan, apakah papan acak belum mencapai goal?
				while(nilai_manhattan !=0 && papan_acak != papan_goal){						// proses penyelesaian puzzle
					timer_mulai = clock();							// mulai menghitung waktu
					indeks_gerakan = astar(0, cost);						// lakukan pergerakan menggunakan heuristik manhattan
					if(indeks_gerakan == 0){						// untuk mendeteksi gerakan yang dilakukan fungsi astar
						strcpy(gerakan, "UP");
					}
					else if(indeks_gerakan == 1){
						strcpy(gerakan, "DOWN");
					}
					else if(indeks_gerakan == 2){
						strcpy(gerakan, "RIGHT");
					}
					else if(indeks_gerakan == 3){
						strcpy(gerakan, "LEFT");
					}
					else{
						printf("\n\nStep A* : Puzzle unsolved\n");
						break;
					} 
					printf("\n\n%s\n", gerakan);
					cetak_papan(papan_acak);
					
					timer_selesai = clock();							// catat waktu yang dihabiskan dalam sekali gerak
					total_waktu += (timer_selesai - timer_mulai);		// total waktu yang digunakan untuk menyelesaikan puzzle
					
					nilai_manhattan = manhattan_distance(papan_acak);
					kedalaman++;
				}
	
				printf("\n\nAlgoritma berjalan selama : %f Detik, dengan kedalaman-%d, cost-%d", ((float)total_waktu)/CLOCKS_PER_SEC, kedalaman, cost);
				if(kedalaman>0) printf("\nNilai effective branching factor : %f", effective_branching_factor(kedalaman));
				printf("\n\nTekan enter untuk lanjut");
				getch();
				break;
			case 2 :										// semua sama dengan case 1, hanya berbeda fungsi heuristik
				acak_papan();
				system("CLS");
				printf("\nPENCARIAN A* DENGAN HEURISTIK BUATAN SENDIRI\n");
				printf("\n==== Initial Board Configuration ====\n\n");
				cetak_papan(papan_acak);
				printf("\n=====================================\n");
				nilai_custom = custom_distance(papan_acak);
				while(nilai_custom != 25 && papan_acak != papan_goal){						// proses penyelesaian puzzle
					timer_mulai = clock();
					indeks_gerakan = astar(1, cost);
					if(indeks_gerakan == 0){
						strcpy(gerakan, "UP");
					}
					else if(indeks_gerakan == 1){
						strcpy(gerakan, "DOWN");
					}
					else if(indeks_gerakan == 2){
						strcpy(gerakan, "RIGHT");
					}
					else if(indeks_gerakan == 3){
						strcpy(gerakan, "LEFT");
					}
					else{
						printf("\n\nPuzzle unsolved\n");
						break;
					}
					printf("\n\n%s\n", gerakan);
					cetak_papan(papan_acak);
					
					timer_selesai = clock();
					total_waktu += (timer_selesai - timer_mulai);
					nilai_custom = custom_distance(papan_acak);
					kedalaman++;					
				}
				
				printf("\n\nAlgoritma berjalan selama : %f Detik, dengan kedalaman-%d", ((float)total_waktu)/CLOCKS_PER_SEC, kedalaman);
				if(kedalaman>0) printf("\nNilai effective branching factor : %f", effective_branching_factor(kedalaman));
				printf("\n\nTekan enter untuk lanjut");
				getch();
				break;
			case 0 :							// untuk keluar dari program
				on = false;
				system("CLS");
				printf("\n\tPROGRAM DIBUAT OLEH :\n\n");
				printf("\n\t1. Anneke Shavira M. (19081010041) \n");
				printf("\n\t2. M. Mushthafainal Akhyar M (19081010042) \n");
				printf("\n\t3. Saiful Abroriy (19081010045) \n");
				printf("\n\t4. Rizqy Ahsana Putri (19081010058) \n");
				printf("\n\t5. Aniisah Eka Rahmawati (19081010063) \n");
				printf("\n\t6. Rifki Setiawan (19081010134) \n");
				break;
			default :
				fflush(stdin);
				system("CLS");
				printf("Menu tidak tersedia");
				printf("\n\nTekan enter untuk lanjut");
				getch();
				break;
		}
	}
}

int acak_papan(){
	int penampung, iterasi, coba;								
	memcpy (papan_acak, papan_goal, 5*5*sizeof(int));	//	Papan goal diambil kemudian diacak posisinya dan disimpan sebagai papan_acak
	indeks_blank[0] = 4;								// untuk menangkap posisi 0 pada papan. pada papan goal posisi 0 ada di pojok kanan bawah
	indeks_blank[1] = 4;
	iterasi = rand() % 20;									// berapa banyak pergerakan random dilakukan
	for(int indeks = 0; indeks<iterasi; indeks++){		//coba2
		if(gerak(indeks) == 0){
			do{
				coba = gerak(rand()%4);								// mengacak arah pergerakan
			}while(coba == 0);
		}
		else gerak(rand()%4);								// mengacak arah pergerakan
	}
	rute[0] = generate_id(papan_acak);					// menyimpan papan hasil pengacakan sebagai node pertama / root
	indeks_rute++;
	jumlah_node++;
	return 0;
}

int cetak_papan(int papan[5][5]){
	for(int baris = 0; baris<5; baris++){
		for(int kolom = 0; kolom<5; kolom++){
			printf("%d\t", papan[baris][kolom]);
		}
		printf("\n");
	}
	
	return 0;
}

int manhattan_distance(int papan[5][5]){
	int posisi_salah, posisi_baris, posisi_kolom, nilai_manhattan = 0;
	
	for(int baris = 0; baris<5; baris++){												// kunjungi satu persatu angka didalam papan
		for(int kolom = 0; kolom<5; kolom++){
			posisi_salah = cari_nilai(papan[baris][kolom]);								// cari dimana posisi seharusnya dari nilai tersebut
			posisi_kolom = posisi_salah % 5;
			posisi_baris = posisi_salah / 5;
			nilai_manhattan += abs(posisi_baris - baris) + abs(posisi_kolom - kolom);	// hitung berapa langkah yang ditempuh untuk mengembalikan ke posisi goal
		}
	}
	return nilai_manhattan;
}

int custom_distance(int papan[5][5]){
	int indeks_benar = 1, posisi_baris, posisi_kolom, nilai_custom = 0;
	
	for(int baris = 0; baris<5; baris++){									// kunjungi tiap kotak pada papan acak
		for(int kolom = 0; kolom<5; kolom++){
			if(indeks_benar == papan[baris][kolom]) nilai_custom++;			// hitung berapa nilai yang sudah menempati posisi seharusnya
			else if(indeks_benar == 25 && papan[4][4] == 0) nilai_custom++;
			indeks_benar++;
		}
	}
	return nilai_custom;
}

int cari_nilai(int nilai){
	int baris = 0, kolom = 0;
	for(int indeks = 0; indeks < 25; indeks++){		// kunjungi tiap kotak papan goal
		if(nilai == papan_goal[baris][kolom]){		// cek apakah nilai pada papan goal sama dengan nilai yang ingin dicari
			return indeks;
			break;
		}
		kolom++;
		if(kolom == 5){								// untuk pindah baris jika mencapai kolom paling kanan
			baris++;
			kolom = 0;
		}
	}
	
	return 0;
}

int cek_gerakan(int gerakan){	// parameter : 0 = atas, 1 = bawah, 2 = kanan, 3 = kiri
	int baris = indeks_blank[0];
	int kolom = indeks_blank[1];
	
	if(gerakan == 0 && baris - 1 >= 0) return 1;		// untuk mengecek apakah pergerakan akan keluar dari papan atau tidak
	else if(gerakan == 1 && baris + 1 < 5) return 1;
	else if(gerakan == 2 && kolom + 1 < 5) return 1;
	else if(gerakan == 3 && kolom - 1 >= 0) return 1;
	else return 0;
}

int gerak(int gerakan){			//0 = atas, 1 = bawah, 2 = kanan, 3 = kiri
	int baris = indeks_blank[0];
	int kolom = indeks_blank[1];
	int penampung;
	
	if(gerakan == 0 && cek_gerakan(0) == 1){				//atas
		penampung = papan_acak[baris-1][kolom];
		papan_acak[baris-1][kolom] = papan_acak[baris][kolom];
		papan_acak[baris][kolom] = penampung;
		indeks_blank[0] -= 1;
		return 1;
	}
	else if(gerakan == 1 && cek_gerakan(1) == 1){			//bawah
		penampung = papan_acak[baris+1][kolom];
		papan_acak[baris+1][kolom] = papan_acak[baris][kolom];
		papan_acak[baris][kolom] = penampung;
		indeks_blank[0] += 1;
		return 1;
	}
	else if(gerakan == 2 && cek_gerakan(2) == 1){			//kanan
		penampung = papan_acak[baris][kolom+1];
		papan_acak[baris][kolom+1] = papan_acak[baris][kolom];
		papan_acak[baris][kolom] = penampung;
		indeks_blank[1] += 1;
		return 1;
	}
	else if(gerakan == 3 && cek_gerakan(3) == 1){			//kiri
		penampung = papan_acak[baris][kolom-1];
		papan_acak[baris][kolom-1] = papan_acak[baris][kolom];
		papan_acak[baris][kolom] = penampung;
		indeks_blank[1] -= 1;
		return 1;
	}
	else return 0;
}

int astar(int heuristik, int &cost){									//0 = manhattan, 1 = buatan sendiri
	int terkecil = 200000000, terbesar = 0;	
	int available = 0;					// untuk mengecek apakah node sudah dikunjungi
	int indeks_terkecil;
	int indeks_terbesar;
	int indeks_gerak;
	int nilai_manhattan;
	int nilai_custom;
	
	switch(heuristik){
		case 0 :
			for(int indeks = 0; indeks < 4; indeks++){		//cek setiap gerakan
				if(cek_gerakan(indeks) == 1){				//cek apakah out of the box
					jumlah_node++;
					indeks_gerak = gerak(indeks);										// gerakkan puzzle lalu cek kemungkinannya
					available = cek_rute(generate_id(papan_acak));
					nilai_manhattan = manhattan_distance(papan_acak) + cost + 1;
					if(available == 1){
						if(nilai_manhattan <= terkecil){					// jika gerakan memungkinkan dan nilai manhattannya kecil, simpan nilai manhattan dan gerakannya
							terkecil = nilai_manhattan;
							indeks_terkecil = indeks;
						}
					}
					if(indeks == 0 && indeks_gerak == 1) gerak(1);							//untuk mengembalikan posisi papan setelah dicek heuristiknya
					else if (indeks == 1 && indeks_gerak == 1) gerak(0);
					else if (indeks == 2 && indeks_gerak == 1) gerak(3);
					else if (indeks == 3 && indeks_gerak == 1) gerak(2);
				}
			}
			
			if(gerak(indeks_terkecil) == 0) {
				return 5;							// bergerak ke nilai manhattan terkecil
			} else{
				rute[indeks_rute] = generate_id(papan_acak);	// simpan ke rute bahwa node ini telah dikunjungi
				indeks_rute++;
				cost++;
				return indeks_terkecil;
			}
			break;
		case 1 :
			for(int indeks = 0; indeks < 4; indeks++){
				if(cek_gerakan(indeks) == 1){				//cek apakah out of the box
					jumlah_node++;
					indeks_gerak = gerak(indeks);	
					available = cek_rute(generate_id(papan_acak));
					nilai_custom = custom_distance(papan_acak);
					if(available == 1){
						if(nilai_custom >= terbesar){				// jika gerakan memungkinkan dan nilai heuristiknya besar, simpan nilai kehuristik dan gerakannya
							terbesar = nilai_custom;
							indeks_terbesar = indeks;
						} 
					}
					if(indeks == 0 && indeks_gerak == 1) gerak(1);							//untuk mengembalikan posisi papan setelah dicek heuristiknya
					else if (indeks == 1 && indeks_gerak == 1) gerak(0);
					else if (indeks == 2 && indeks_gerak == 1) gerak(3);
					else if (indeks == 3 && indeks_gerak == 1) gerak(2);
				}
			}
			if(gerak(indeks_terbesar) == 0) {
				return 5;							// bergerak ke nilai heuristik terbesar
			} else{
				rute[indeks_rute] = generate_id(papan_acak);	// simpan ke rute bahwa node ini telah dikunjungi
				indeks_rute++;
				return indeks_terbesar;
			}
			break;
	}
}

int generate_id(int papan[5][5]){
	int baris = 0, kolom = 0, id = 0;
	for(int indeks = 0; indeks < 25; indeks++){		// tiap kotak pada papan diberi indeks 1-25 mengular
		id += indeks * papan_acak[baris][kolom];	// tiap angka dikalikan indeks kotak yang ia tempati untuk membuat nilai unik yang membedakan antar gerakan pada puzzle
		kolom++;
		if(kolom == 5){
			baris++;
			kolom = 0;
		}
	}
	return id;
}

int cek_rute(int id){											// untuk menghindari perulangan
	int available = 0;
	for(int indeks = 0; indeks <= indeks_rute; indeks++){	// cek satu per satu rute yang tersimpan, bandingkan id unik yang tersimpan dengan id unik yang sedang dicek
		if(rute[indeks] == id){
			available++;
		}
	}
	if(available == 0) return 1;
	else return 0;
}

double effective_branching_factor(int kedalaman) {
	double error_maksimal = 0.01; // the maximum error we accept from a solution 
    double delta = 0.01;    // how much we change our tested ebf each iteration   
    int indeks_error = 0;    // the sign of the difference between N+1 and 1+b+b^2+......+b^d
    double ebf = 0;           // search for the optimum b will start from minimum possible b
    while (true) {        // search for b starts here
            // compute the expression 1+b+b^2+......+b^d
        double total = 1;
        for (int indeks = 1; indeks < kedalaman + 1; indeks++) {
            total += pow(ebf, (double)indeks);
        }
            //compute the difference between N+1 and 1+b+b^2+......+b^d, remember that we should have N+1=1+b+b^2+......+b^d
        double error = total - (1 + (double)jumlah_node);
            // save previous sign of error
        int indeks_error_sebelumnya = indeks_error;
            // determine the new sign of error
        if (error < 0.0) // negative
            indeks_error=-1;
        else // positive
            indeks_error=+1;
            /* if the error calculated above is greater than the maximum acceptable error, then check if sign of error
               was changed, if so that means loop missed the root that we are looking for, then decrease b by delta and 
               decrease delta to catch root in next search if sign of error wasnt change then increase 'b' by delta
               otherwise if error is smaller than the limit return the effective branch factor */
        if (abs(error) > error_maksimal) { // error is big
            if (indeks_error_sebelumnya == indeks_error || indeks_error_sebelumnya == 0) {  
                ebf += delta;   // taking another step towards solution
            } else {        // change of sign which means that we jumped over the minima
                ebf -= delta;   // go back
                delta /= 2;   // take smaller steps
                indeks_error = indeks_error_sebelumnya;  // undo change of sign
            }
        } else // error is small, let's return current estimate
            return(ebf);
    }
}
