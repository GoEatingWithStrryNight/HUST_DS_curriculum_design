let sudokuBoard = [];
let solution = [];
let selectedCell = null;
let timer = null;
let seconds = 0;
let errors = 0;

function startGame(difficulty) {
    console.log('开始游戏，难度：', difficulty);
    document.getElementById('difficulty-selection').style.display = 'none';
    document.getElementById('game-board').style.display = 'block';
    document.getElementById('game-result').style.display = 'none';

    // 移除所有已存在的返回菜单按钮
    removeAllReturnButtons();

    generateSudoku(difficulty).then(() => {
        console.log('数独生成成功');
        createGrid();
        createNumberPad();
        startTimer();
        errors = 0;
        updateErrorDisplay();
    }).catch(error => {
        console.error('Error generating Sudoku:', error);
        alert('生成数独时出错，请重试。');
        returnToMenu();
    });
}

function generateSudoku(difficulty) {
    // 这里模拟从后端获取数独题目
    return new Promise((resolve) => {
        setTimeout(() => {
            const difficultyLevel = difficulty === 'easy' ? 1 : (difficulty === 'medium' ? 2 : 3);
            sudokuBoard = [
                [5, 3, 0, 0, 7, 0, 0, 0, 0],
                [6, 0, 0, 1, 9, 5, 0, 0, 0],
                [0, 9, 8, 0, 0, 0, 0, 6, 0],
                [8, 0, 0, 0, 6, 0, 0, 0, 3],
                [4, 0, 0, 8, 0, 3, 0, 0, 1],
                [7, 0, 0, 0, 2, 0, 0, 0, 6],
                [0, 6, 0, 0, 0, 0, 2, 8, 0],
                [0, 0, 0, 4, 1, 9, 0, 0, 5],
                [0, 0, 0, 0, 8, 0, 0, 7, 9]
            ];

            // 生成完整的解决方案
            solution = generateFullSudoku();

            // 根据难度挖洞
            const holes = difficulty === 'easy' ? 30 : (difficulty === 'medium' ? 40 : 50);
            sudokuBoard = digHoles(solution, holes);

            resolve();
        }, 500);
    });
}

function generateFullSudoku() {
    // 这个函数应该生成一个有效的完整数独解决方案
    // 这里使用一个简单的示例，实际应用中应该使用更复杂的算法
    return [
        [5, 3, 4, 6, 7, 8, 9, 1, 2],
        [6, 7, 2, 1, 9, 5, 3, 4, 8],
        [1, 9, 8, 3, 4, 2, 5, 6, 7],
        [8, 5, 9, 7, 6, 1, 4, 2, 3],
        [4, 2, 6, 8, 5, 3, 7, 9, 1],
        [7, 1, 3, 9, 2, 4, 8, 5, 6],
        [9, 6, 1, 5, 3, 7, 2, 8, 4],
        [2, 8, 7, 4, 1, 9, 6, 3, 5],
        [3, 4, 5, 2, 8, 6, 1, 7, 9]
    ];
}

function digHoles(fullSudoku, holes) {
    let board = JSON.parse(JSON.stringify(fullSudoku)); // 深拷贝
    let positions = [];
    for (let i = 0; i < 9; i++) {
        for (let j = 0; j < 9; j++) {
            positions.push([i, j]);
        }
    }

    // 随机挖洞
    for (let i = 0; i < holes; i++) {
        if (positions.length === 0) break;
        let index = Math.floor(Math.random() * positions.length);
        let [row, col] = positions.splice(index, 1)[0];
        board[row][col] = 0;
    }

    return board;
}

function createGrid() {
    console.log('开始创建网格');
    const grid = document.getElementById('sudoku-grid');
    grid.innerHTML = '';
    for (let i = 0; i < 9; i++) {
        for (let j = 0; j < 9; j++) {
            const cell = document.createElement('div');
            cell.className = 'cell';
            if (i === j || i + j === 8) {
                cell.classList.add('diagonal');
            }
            if (sudokuBoard[i][j] !== 0) {
                cell.textContent = sudokuBoard[i][j];
                cell.classList.add('fixed');
            } else {
                cell.addEventListener('click', () => selectCell(cell, i, j));
            }
            grid.appendChild(cell);
        }
    }
    console.log('网格创建完成');

    // 添加返回菜单按钮
    addReturnButton();
}

function addReturnButton() {
    // 检查是否已存在返回按钮，如果存在则不添加新的
    if (!document.getElementById('return-button')) {
        const returnButton = document.createElement('button');
        returnButton.id = 'return-button';
        returnButton.textContent = '返回菜单';
        returnButton.onclick = returnToMenu;
        document.querySelector('.container').appendChild(returnButton);
    }
}

function removeAllReturnButtons() {
    const returnButtons = document.querySelectorAll('#return-button');
    returnButtons.forEach(button => button.remove());

    // 也移除可能存在于 .container 中的按钮
    const containerButtons = document.querySelectorAll('.container > #return-button');
    containerButtons.forEach(button => button.remove());
}

function createNumberPad() {
    const numberPad = document.getElementById('number-pad');
    numberPad.innerHTML = '';
    for (let i = 1; i <= 9; i++) {
        const button = document.createElement('button');
        button.textContent = i;
        button.addEventListener('click', () => {
            console.log('数字按钮被点击:', i);
            fillNumber(i);
        });
        numberPad.appendChild(button);
    }
    console.log('数字面板创建完成');
}

function selectCell(cell, row, col) {
    console.log('选中单元格:', row, col);
    // 移除所有单元格的 'selected' 类
    document.querySelectorAll('.cell').forEach(c => c.classList.remove('selected'));

    // 为选中的单元格添加 'selected' 类
    cell.classList.add('selected');

    // 更新 selectedCell 变量
    selectedCell = cell;
    console.log('selectedCell 已更新:', selectedCell);

    highlightRelatedCells(row, col);
}

function highlightRelatedCells(row, col) {
    const cells = document.querySelectorAll('.cell');
    cells.forEach((cell, index) => {
        const cellRow = Math.floor(index / 9);
        const cellCol = index % 9;
        if (cellRow === row || cellCol === col ||
            (Math.floor(cellRow / 3) === Math.floor(row / 3) &&
                Math.floor(cellCol / 3) === Math.floor(col / 3))) {
            cell.classList.add('highlighted');
        } else {
            cell.classList.remove('highlighted');
        }
    });
}

function fillNumber(number) {
    console.log('尝试填入数字:', number);
    console.log('selectedCell:', selectedCell);
    if (selectedCell && !selectedCell.classList.contains('fixed')) {
        const row = Math.floor([...selectedCell.parentNode.children].indexOf(selectedCell) / 9);
        const col = [...selectedCell.parentNode.children].indexOf(selectedCell) % 9;
        
        if (isValidPlacement(sudokuBoard, row, col, number) && solution[row][col] === number) {
            selectedCell.textContent = number;
            selectedCell.classList.add('correct');
            sudokuBoard[row][col] = number;
            checkGameCompletion();
        } else {
            // 不填入错误的数字，只显示错误状态
            selectedCell.classList.add('incorrect');
            setTimeout(() => {
                selectedCell.classList.remove('incorrect');
            }, 500);
            errors++;
            updateErrorDisplay();
            if (errors >= 3) {
                endGame(false);
            }
        }
        console.log('数字已处理');
    } else {
        console.log('无法填入数字：未选中单元格或单元格为固定值');
    }
}

function updateErrorDisplay() {
    document.getElementById('errors').textContent = `错误: ${errors}/3`;
}

function checkGameCompletion() {
    const allCells = document.querySelectorAll('.cell');
    const isComplete = [...allCells].every(cell => cell.textContent !== '');
    if (isComplete) {
        endGame(true);
    }
}

function endGame(isWin) {
    clearInterval(timer);
    document.getElementById('game-board').style.display = 'none';

    // 移除所有已存在的返回菜单按钮
    removeAllReturnButtons();

    const gameResult = document.getElementById('game-result');
    gameResult.innerHTML = ''; // 清空之前的内容
    gameResult.style.display = 'block';

    const resultMessage = document.createElement('div');
    resultMessage.id = 'result-message';
    if (isWin) {
        resultMessage.textContent = `恭喜！你赢了！用时: ${formatTime(seconds)}`;
    } else {
        resultMessage.textContent = '游戏结束，错误次数超过3次。';
    }
    gameResult.appendChild(resultMessage);

    // 为结果界面添加返回菜单按钮
    const returnButton = document.createElement('button');
    returnButton.id = 'return-button';
    returnButton.textContent = '返回菜单';
    returnButton.onclick = returnToMenu;
    gameResult.appendChild(returnButton);
}

function startTimer() {
    if (timer) clearInterval(timer);
    seconds = 0;
    timer = setInterval(() => {
        seconds++;
        document.getElementById('timer').textContent = `时间: ${formatTime(seconds)}`;
    }, 1000);
}

function formatTime(totalSeconds) {
    const minutes = Math.floor(totalSeconds / 60);
    const seconds = totalSeconds % 60;
    return `${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
}

function returnToMenu() {
    if (timer) clearInterval(timer);
    document.getElementById('difficulty-selection').style.display = 'block';
    document.getElementById('game-board').style.display = 'none';
    document.getElementById('game-result').style.display = 'none';

    // 移除所有返回菜单按钮
    removeAllReturnButtons();

    // 重置游戏状态
    selectedCell = null;
    errors = 0;
    seconds = 0;
    updateErrorDisplay();
    document.getElementById('timer').textContent = '时间: 00:00';
}

function isValidPlacement(board, row, col, num) {
    // 检查行
    for (let x = 0; x < 9; x++) {
        if (board[row][x] == num) return false;
    }
    
    // 检查列
    for (let x = 0; x < 9; x++) {
        if (board[x][col] == num) return false;
    }
    
    // 检查3x3方格
    let startRow = Math.floor(row / 3) * 3;
    let startCol = Math.floor(col / 3) * 3;
    for (let i = 0; i < 3; i++) {
        for (let j = 0; j < 3; j++) {
            if (board[i + startRow][j + startCol] == num) return false;
        }
    }
    
    return true;
}

// 在实际项目中，您需要添加更多函数来处理游戏逻辑，如检查答案、完成游戏等