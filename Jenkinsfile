/* Jenkinsfile - declarative pipeline */
/* https://jenkins.io/doc/book/pipeline/ */
/* https://jenkins.io/doc/book/pipeline/syntax */

pipeline {
    agent any
    environment {
        DIMS_DEBUG=1
    }
    stages {
        stage('Pre-Build') {
            steps {
                sh '[ $DIMS_DEBUG ] && env'
        }
        stage('Build') {
            steps {
                sh '(cd docs; jenkins.shell make latexpdf)'
            }
        }
        stage('Deploy') {
            steps {
                sh '(cd docs/build/latex; cp *.pdf /tmp)'
            }
        }
    }
    post {
        always {
            /* Executed regardless of completion status. */
            echo 'POST'
        }
        changed {
            /* Only executed if status of current run differs from previous run. */
           echo 'CHANGED'
        }
        failure {
            /* Only executed if current status is "failed". */
           echo 'FAILURE'
        }
        success {
            /* Only executed if current status is "success". */
           echo 'SUCCESS'
        }
        unstable {
            /* Only executed if current status is "unstable" (e.g. when tests fail.) */
           echo 'UNSTABLE'
        }
    }
}

# vim: set ts=4 sw=4 tw=0 et :
