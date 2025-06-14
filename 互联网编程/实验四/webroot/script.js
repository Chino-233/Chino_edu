document.addEventListener('DOMContentLoaded', function() {
    console.log("页面加载完成");
    
    // 表单验证
    const form = document.querySelector('form');
    if (form) {
        form.addEventListener('submit', function(event) {
            const username = document.getElementById('username');
            const password = document.getElementById('password');
            
            if (username && password) {
                if (!username.value.trim()) {
                    alert('请输入用户名');
                    username.focus();
                    event.preventDefault();
                    return;
                }
                
                if (!password.value.trim()) {
                    alert('请输入密码');
                    password.focus();
                    event.preventDefault();
                    return;
                }
                
                // 显示提交消息
                console.log('表单提交中...');
            }
        });
    }
    
    // 获取Cookie函数
    function getCookie(name) {
        const value = `; ${document.cookie}`;
        const parts = value.split(`; ${name}=`);
        if (parts.length === 2) return parts.pop().split(';').shift();
        return null;
    }
    
    // 如果有username cookie，显示欢迎消息
    const username = getCookie('username');
    if (username) {
        const container = document.querySelector('.container');
        if (container) {
            const welcomeMessage = document.createElement('div');
            welcomeMessage.className = 'welcome-message';
            welcomeMessage.style.backgroundColor = '#e7f3fe';
            welcomeMessage.style.padding = '10px';
            welcomeMessage.style.marginBottom = '20px';
            welcomeMessage.style.borderRadius = '4px';
            welcomeMessage.style.borderLeft = '4px solid #2196F3';
            welcomeMessage.innerHTML = '<p>欢迎回来, <strong>' + username + '</strong>! 您已登录。</p>';
            
            const firstChild = container.firstChild;
            container.insertBefore(welcomeMessage, firstChild);
        }
    }
    
    // 图片加载失败处理
    const images = document.querySelectorAll('img');
    images.forEach(function(img) {
        img.addEventListener('error', function() {
            this.src = 'error.png'; // 可以替换为默认的错误图片
            this.alt = '图片加载失败';
        });
    });
    
    // 处理导航栏的登录状态
    const loginLink = document.getElementById('login-link');
    const registerLink = document.getElementById('register-link');
    const logoutLink = document.getElementById('logout-link');
    
    if (username && loginLink && registerLink && logoutLink) {
        console.log("用户已登录:", username);
        loginLink.style.display = 'none';
        registerLink.style.display = 'none';
        logoutLink.style.display = 'inline-block';
    } else {
        console.log("用户未登录");
        if (loginLink) loginLink.style.display = 'inline-block';
        if (registerLink) registerLink.style.display = 'inline-block';
        if (logoutLink) logoutLink.style.display = 'none';
    }
});