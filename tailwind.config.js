/** @type {import('tailwindcss').Config} */
module.exports = {
  content: ["./src/web/*.html"],
  theme: {
    extend: {},
  },
  plugins: [
    require('daisyui')
  ],
}

