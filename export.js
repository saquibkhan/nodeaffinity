const axios = require("axios");
const XLSX = require("xlsx");
const token = process.env.GITHUB_TOKEN;
const headers = {
  "Authorization": `Token ${token}`
};
const repoName = "nodeaffinity";
const repoOwner = "saquibkhan";
axios.get(`https://api.github.com/repos/${repoOwner}/${repoName}/issues`, { headers })
 .then(response => {
 console.log("************** response data *******************");
  console.log(response.data);
  const workbook = XLSX.utils.json_to_sheet(response.data);
   console.log("************** workbook *******************");
  console.log(workbook);
XLSX.writeFile(XLSX.utils.book_new([workbook]), "issues.xlsx");});
