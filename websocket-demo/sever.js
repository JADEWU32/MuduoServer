const boxesContainer = document.getElementById('boxes');
const receiveDataBtn = document.getElementById('receiveDataBtn');
const clearCacheBtn = document.getElementById('clearCacheBtn');

let dataStorage = {}; // 用于存储接收到的数据

function addBox(category) {
    const box = document.createElement('div');
    box.className = 'box';
    box.setAttribute('data-category', category);

    const boxHeader = document.createElement('div');
    boxHeader.className = 'box-header';

    const title = document.createElement('span');
    title.textContent = `${category} 号连接`;
    
    const clearButton = document.createElement('button');
    clearButton.textContent = '清屏';
    clearButton.className = 'clear-button';
    clearButton.onclick = () => clearBox(box);

    const boxcontent = document.createElement('div');
    boxcontent.className = 'box-content';
    
    boxHeader.appendChild(title);
    boxHeader.appendChild(clearButton);
    box.appendChild(boxHeader);
    box.appendChild(boxcontent); // 用于显示数据
    boxesContainer.appendChild(box);
}

function clearBox(box) {
    const contentDiv = box.querySelector('div:nth-child(2)');
    contentDiv.textContent = ''; // 清空内容
}

async function receiveData() {
    try {
        const response = await fetch('http://localhost:8080/getdata'); // 请求后端服务器
        if (!response.ok) throw new Error('Network response was not ok');
        
        const data = await response.json(); // 获取 JSON 数据
        classifyAndDisplayMessages(data.data); // 分类并显示信息
    } catch (error) {
        console.error('There has been a problem with your fetch operation:', error);
    }
}
        
function classifyAndDisplayMessages(messages) {

    messages.forEach(message => {
        const { category, message: msgContent } = message;

        // 检查该分类是否已经存在，如果不存在则初始化一个数组
        if (!dataStorage[category]) {
            dataStorage[category] = [];
            if (!Array.from(boxesContainer.children).find(b => b.dataset.category === category)) {
                addBox(category); // 新增方框
            }
        }

        // 将消息添加到对应分类中
        dataStorage[category].push(msgContent);
        updateBoxContent(category);
    });
}

function updateBoxContent(category) {
    const box = Array.from(boxesContainer.children).find(b => b.dataset.category === category);
    const contentDiv = box.querySelector('div:nth-child(2)');
    contentDiv.textContent += dataStorage[category] + '\n'; // 更新内容
    dataStorage[category] = '';
    contentDiv.scrollTop = contentDiv.scrollHeight;
}

function clearCache() {
    dataStorage = {};
    // boxesContainer.innerHTML = ''; // 清空方框
    // for (let i = 0; i < 6; i++) {
    //     addBox(String.fromCharCode(65 + i)); // 初始化6个方框
    // }
    clearrrrrr();
}

async function clearrrrrr() {
    try {
        const response = await fetch('http://localhost:8080/cleardata'); // 请求后端服务器
        if (!response.ok) throw new Error('Network response was not ok');
        const data = await response.json(); // 获取 JSON 数据
    } catch (error) {
        console.error('There has been a problem with your fetch operation:', error);
    }
}

// 事件监听
receiveDataBtn.onclick = receiveData;
clearCacheBtn.onclick = clearCache;

// // 初始化6个方框
// for (let i = 0; i < 6; i++) {
//     addBox(String.fromCharCode(65 + i)); // A - F
// }
