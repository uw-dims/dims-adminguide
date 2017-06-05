pipeline {
  agent any

  stages {
    stage('Build') {
      steps {
        sh 'env'
        sh '(cd docs; jenkins.shell make latexpdf)'
      }
    }
    stage('Deploy') {
      steps {
        sh '(cd docs/build/latex; cp *.pdf /tmp)'
      }
    }
  }
}
