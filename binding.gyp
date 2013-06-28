{
  'targets': [
    {
      'target_name': 'node_uuid',
      'sources': [ 
        'src/node_uuid.cc'
      ],
      'link_settings': {
        'libraries': [ '-luuid' ]
      }
    }
  ]
}
