const vscode = require('vscode');

function hello() {
  vscode.window.showInformationMessage('hello');
}

async function activate(context) {
  console.log(activate, context);
  let disposable = vscode.commands.registerCommand('pf.hello', hello);
  context.subscriptions.push(disposable);
}

function deactivate() {
  console.log(deactivate);
}

module.exports = {
  activate,
  deactivate
}
