#include "water_seven.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////
//Task 0
bool readInput(
    const string &filename,
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], 
    int skill[FIXED_CHARACTER], int &shipHP, int &repairCost){
        ifstream fin(filename);
    if (!fin.is_open()) return false;

    string line;
    int idx = 0;

    while (getline(fin, line))
    {
        if (line == "") continue;  

        string name;
        int a, b;
        stringstream ss(line);
        ss >> name >> a >> b;

        if (name == "GOING_MERRY")  
        {
            shipHP = a;
            repairCost = b;
        }
        else   // dòng nhân vật
        {
            strcpy(character[idx], name.c_str());
            hp[idx] = a;
            skill[idx] = b;
            idx++;
        }
    }

    fin.close();
    return true;
}



// Task 1
bool perfect_number(int n){
    if(n <= 1) return false;
    int sum = 1;  
    for(int i = 2; i * i <= n; i++){
        if(n % i == 0){
            sum += i;
            if(i != n / i){
                sum += n / i;
            }
        }
    }
    return sum == n;
}
int sum_digits(int n){
    int sum=0;
    while(n>0){
        sum+=n%10;
        n/=10;
    }
    return sum;
}
int damageEvaluation(int shipHP, int repairCost){
    if (shipHP < 455 && perfect_number(sum_digits(shipHP))) {
        repairCost = (int)round(repairCost * 1.5);
    }
    return repairCost;
}

// Task 2
int conflictSimulation(
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER],
    int shipHP, int repairCost){
         int LUFFY = -1, USOPP = -1;

    for (int i = 0; i < FIXED_CHARACTER; i++) {
        if (strcmp(character[i], "LUFFY") == 0) LUFFY = i;
        if (strcmp(character[i], "USOPP") == 0) USOPP = i;
    }

    if (LUFFY == -1 || USOPP == -1) return -1;

    int conflictIndex = skill[LUFFY] - skill[USOPP]
                  + repairCost / 100
                  + (500 - shipHP) / 50;

int eventCount = 0;

while (conflictIndex < 255 && eventCount < 10) {
    int id = conflictIndex % 6;

    if (id == 0) conflictIndex += 255;
    else if (id == 1) conflictIndex += 20;
    else if (id == 2) conflictIndex += 50;
    else if (id == 3) conflictIndex += 70;
    else if (id == 4) conflictIndex += 90;
    else if (id == 5) conflictIndex += 100;

    eventCount++;
}
    return conflictIndex;
    }

// Task 3
void resolveDuel(
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER],
    int conflictIndex, int repairCost, char duel[FIXED_CHARACTER][MAX_NAME]){
        // Initialize output to empty strings
    for (int i = 0; i < FIXED_CHARACTER; ++i) {
        duel[i][0] = '\0';
    }

    // Find indices of LUFFY and USOPP
    int idxL = -1, idxU = -1;
    for (int i = 0; i < FIXED_CHARACTER; ++i) {
        if (strcmp(character[i], "LUFFY") == 0) idxL = i;
        if (strcmp(character[i], "USOPP") == 0) idxU = i;
    }

    if (idxL == -1 || idxU == -1) {
        // Required characters not found; leave duel empty
        return;
    }

    // Compute U as integer expression per specification
    int U = skill[idxU] + (conflictIndex / 20) + (repairCost / 500);

    // If Luffy already strong enough, no helpers needed
    if (skill[idxL] >= U) {
        return;
    }

    // Build list of member indices excluding Luffy and Usopp
    int members[FIXED_CHARACTER];
    int m = 0;
    for (int i = 0; i < FIXED_CHARACTER; ++i) {
        if (i == idxL || i == idxU) continue;
        members[m++] = i;
    }

    // Precompute supports and costs for these members
    int support[FIXED_CHARACTER];
    int cost[FIXED_CHARACTER];
    for (int i = 0; i < m; ++i) {
        int idx = members[i];
        support[i] = skill[idx];
        cost[i] = (hp[idx] % 10) + 1;
    }

    // Brute force all subsets of the m members (m should be 5)
    int bestMask = 0;
    int bestCost = INT_MAX;
    int bestCount = INT_MAX;
    int totalMasks = 1 << m;
    for (int mask = 1; mask < totalMasks; ++mask) {
        int sumSupport = 0;
        int sumCost = 0;
        int cnt = 0;
        for (int b = 0; b < m; ++b) {
            if (mask & (1 << b)) {
                sumSupport += support[b];
                sumCost += cost[b];
                ++cnt;
            }
        }
        if (skill[idxL] + sumSupport >= U) {
            if (sumCost < bestCost || (sumCost == bestCost && cnt < bestCount)) {
                bestCost = sumCost;
                bestCount = cnt;
                bestMask = mask;
            }
        }
    }

    if (bestCost == INT_MAX) {
        // No feasible subset found; leave duel empty
        return;
    }

    // Populate duel array with selected members' names in original order
    int out = 0;
    for (int b = 0; b < m; ++b) {
        if (bestMask & (1 << b)) {
            int idx = members[b];
            // copy name
            strcpy(duel[out], character[idx]);
            ++out;
        }
    }
    }

// Task 4
void decodeCP9Message(char character[FIXED_CHARACTER][MAX_NAME], 
    int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER], int conflictIndex, 
    int repairCost, char cipherText[], char resultText[])
{
    // ===== 1. Tách message và checksum =====
    char message[1000];
    int msgLen = 0;
    int i = 0;

    while (cipherText[i] != '#' && cipherText[i] != '\0') {
        message[msgLen++] = cipherText[i++];
    }
    message[msgLen] = '\0';

    if (cipherText[i] != '#') {
        resultText[0] = '\0';
        return;
    }

    int checksumInput = (cipherText[i + 1] - '0') * 10 + (cipherText[i + 2] - '0');

    // ===== 2. Kiểm tra checksum =====
    int checksum = 0;
    for (int j = 0; j < msgLen; j++) {
        checksum += (int)message[j];
    }
    checksum %= 100;

    if (checksum != checksumInput) {
        resultText[0] = '\0';
        return;
    }

    // ===== 3. Đảo theo block =====
    int key = (conflictIndex + repairCost) % 26;
    int B = (key % 5) + 4;

    char reversed[1000];
    int pos = 0;

    for (int start = 0; start < msgLen; start += B) {
        int end = start + B - 1;
        if (end >= msgLen) end = msgLen - 1;

        for (int j = end; j >= start; j--) {
            reversed[pos++] = message[j];
        }
    }
    reversed[pos] = '\0';

    // ===== 4. Giải mã dịch vòng =====
    int digitShift = key % 10;

    for (int j = 0; reversed[j] != '\0'; j++) {
        char c = reversed[j];

        if (c >= 'A' && c <= 'Z') {
            c = c - key;
            while (c < 'A') c += 26;
        }
        else if (c >= 'a' && c <= 'z') {
            c = c - key;
            while (c < 'a') c += 26;
        }
        else if (c >= '0' && c <= '9') {
            c = c - digitShift;
            while (c < '0') c += 10;
        }

        resultText[j] = c;
    }
    resultText[pos] = '\0';

    // ===== 5. Kiểm tra hợp lệ =====
    bool valid = false;

    for (int j = 0; resultText[j] != '\0'; j++) {
        if (resultText[j] == 'C' && resultText[j + 1] == 'P' && resultText[j + 2] == '9') {
            valid = true;
            break;
        }
    }

    for (int j = 0; resultText[j] != '\0'; j++) {
        if (resultText[j] == 'E') {
            int k = 0;
            char keyStr[] = "ENIESLOBBY";
            while (keyStr[k] != '\0' && resultText[j + k] == keyStr[k]) k++;
            if (keyStr[k] == '\0') {
                valid = true;
                break;
            }
        }
    }

    // ===== 6. Gắn TRUE / FALSE =====
    int len = 0;
    while (resultText[len] != '\0') len++;

    if (valid) {
        resultText[len++] = '_';
        resultText[len++] = 'T';
        resultText[len++] = 'R';
        resultText[len++] = 'U';
        resultText[len++] = 'E';
    } else {
        resultText[len++] = '_';
        resultText[len++] = 'F';
        resultText[len++] = 'A';
        resultText[len++] = 'L';
        resultText[len++] = 'S';
        resultText[len++] = 'E';
    }
    resultText[len] = '\0';
}

// Task 5
int analyzeDangerLimit(int grid[MAX_GRID][MAX_GRID], int rows, int cols)
{
    int maxRowSum = 0;
    int maxCell = INT_MIN;

    for (int i = 0; i < rows; ++i) {
        int rowSum = 0;
        for (int j = 0; j < cols; ++j) {
            int v = grid[i][j];
            if (v >= 0) rowSum += v; // ignore -1 cells
            if (v > maxCell) maxCell = v;
        }
        if (rowSum > maxRowSum) maxRowSum = rowSum;
    }

    if (maxCell == INT_MIN) maxCell = 0;

    return maxRowSum + maxCell;
}

bool evaluateRoute(int grid[MAX_GRID][MAX_GRID], int rows, int cols, int dangerLimit)
{
    const int INF = 1000000000;

    if (rows <= 0 || cols <= 0) return false;
    if (grid[0][0] == -1 || grid[rows-1][cols-1] == -1) return false;

    static int dp[MAX_GRID][MAX_GRID];
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            dp[i][j] = INF;

    dp[0][0] = (grid[0][0] >= 0) ? grid[0][0] : INF;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == -1) continue;
            if (i > 0 && dp[i-1][j] != INF) {
                dp[i][j] = min(dp[i][j], dp[i-1][j] + grid[i][j]);
            }
            if (j > 0 && dp[i][j-1] != INF) {
                dp[i][j] = min(dp[i][j], dp[i][j-1] + grid[i][j]);
            }
        }
    }

    if (dp[rows-1][cols-1] == INF) return false;

    return dp[rows-1][cols-1] <= dangerLimit;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
