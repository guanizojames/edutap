# EduTap

EduTap is an educational platform designed to streamline learning, assessment, and student engagement. This README provides an overview of the project structure, installation instructions, features, and development guidelines.

## 🚀 Features

* VB.NET desktop application interface
* Arduino microcontroller integration
* Serial communication (read/write COM port)
* Real-time sensor display
* Device control via VB buttons (LEDs, motors, modules)
* Error handling and connection status monitoring
* User authentication and role management
* Course creation and enrollment
* Interactive lessons and modules
* Quizzes, assessments, and grading
* Real-time analytics and progress tracking
* Admin dashboard for managing content and users

## 📦 Project Structure

```
edutap/
├── src/
│   ├── components/
│   ├── pages/
│   ├── services/
│   ├── utils/
│   ├── assets/
│   └── styles/
├── public/
├── README.md
├── package.json
└── ...
```

## 🛠️ Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/your-username/edutap.git
   ```
2. Navigate to the project directory:

   ```bash
   cd edutap
   ```
3. Install dependencies:

   ```bash
   npm install
   ```
4. Start the development server:

   ```bash
   npm run dev
   ```

## ⚙️ Environment Variables

Create a `.env` file in the root directory and include:

```
VITE_API_URL=your_api_url
VITE_FIREBASE_KEY=your_firebase_key
```

## 📚 Tech Stack

* **Application:** Visual Basic .NET (VB.NET)
* **Microcontroller:** Arduino (C/C++)
* **Communication:** Serial (USB) communication between VB app and Arduino
* **Database:** (Specify if using MySQL / SQLite / none)
* **UI Framework:** Windows Forms

## 🧪 Testing

```bash
npm run test
```

## 🛡️ Security & Best Practices

* Always validate user input
* Store secrets in environment variables
* Use secure authentication (JWT/OAuth/Firebase)
* Follow proper access control

## 🤝 Contributing

1. Fork the repo
2. Create a new branch
3. Commit changes
4. Submit a pull request
